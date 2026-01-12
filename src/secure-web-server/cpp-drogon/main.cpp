#include <drogon/drogon.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpController.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/utils/Utilities.h>
#include <regex>
#include <unordered_map>
#include <chrono>
#include <mutex>

using namespace drogon;

// Rate limiting
class RateLimiter {
private:
    std::unordered_map<std::string, std::vector<std::chrono::steady_clock::time_point>> requests;
    std::mutex mtx;
    const size_t maxRequests = 10;
    const std::chrono::seconds window{60};

public:
    bool isAllowed(const std::string& ip) {
        std::lock_guard<std::mutex> lock(mtx);
        auto now = std::chrono::steady_clock::now();
        auto& clientRequests = requests[ip];
        
        clientRequests.erase(
            std::remove_if(clientRequests.begin(), clientRequests.end(),
                [this, now](const auto& time) {
                    return now - time > window;
                }),
            clientRequests.end()
        );
        
        if (clientRequests.size() >= maxRequests) {
            return false;
        }
        
        clientRequests.push_back(now);
        return true;
    }
};

RateLimiter rateLimiter;

// Input validation
std::string sanitizeInput(const std::string& input) {
    std::string sanitized = input;
    std::regex htmlTags("<[^>]*>");
    sanitized = std::regex_replace(sanitized, htmlTags, "");
    
    std::unordered_map<std::string, std::string> entities = {
        {"&", "&amp;"}, {"<", "&lt;"}, {">", "&gt;"}, 
        {"\"", "&quot;"}, {"'", "&#x27;"}, {"/", "&#x2F;"}
    };
    
    for (const auto& [ch, entity] : entities) {
        size_t pos = 0;
        while ((pos = sanitized.find(ch, pos)) != std::string::npos) {
            sanitized.replace(pos, ch.length(), entity);
            pos += entity.length();
        }
    }
    
    return sanitized;
}

bool isValidEmail(const std::string& email) {
    std::regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return std::regex_match(email, emailRegex) && email.length() <= 254;
}

// Controllers
class HomeController : public HttpController<HomeController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(HomeController::home, "/", Get);
    METHOD_LIST_END

    void home(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
        if (!rateLimiter.isAllowed(req->peerAddr().toIp())) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k429TooManyRequests);
            resp->setBody("Rate limit exceeded");
            callback(resp);
            return;
        }

        std::string html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Home</title>
    <link rel="stylesheet" href="/static/style.css">
</head>
<body>
    <div class="container">
        <h1>Hello. Welcome to the Home Page.</h1>
        <a href="/submit" class="button">Submit Message</a>
    </div>
</body>
</html>)";
        
        auto resp = HttpResponse::newHttpResponse();
        resp->setContentTypeCode(CT_TEXT_HTML);
        resp->setBody(html);
        callback(resp);
    }
};

class SubmitController : public HttpController<SubmitController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(SubmitController::showForm, "/submit", Get);
    ADD_METHOD_TO(SubmitController::handleSubmit, "/submit", Post);
    METHOD_LIST_END

    void showForm(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
        if (!rateLimiter.isAllowed(req->peerAddr().toIp())) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k429TooManyRequests);
            resp->setBody("Rate limit exceeded");
            callback(resp);
            return;
        }

        std::string html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Submit Message</title>
    <link rel="stylesheet" href="/static/style.css">
</head>
<body>
    <div class="container">
        <h1>Submit Message</h1>
        <form method="post" action="/submit">
            <div class="form-group">
                <label for="email">Email:</label>
                <input type="email" id="email" name="email" required maxlength="254">
            </div>
            <div class="form-group">
                <label for="message">Message:</label>
                <textarea id="message" name="message" required maxlength="1000"></textarea>
            </div>
            <button type="submit" class="button">Submit</button>
        </form>
        <a href="/" class="link">Back to Home</a>
    </div>
</body>
</html>)";
        
        auto resp = HttpResponse::newHttpResponse();
        resp->setContentTypeCode(CT_TEXT_HTML);
        resp->setBody(html);
        callback(resp);
    }

    void handleSubmit(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
        if (!rateLimiter.isAllowed(req->peerAddr().toIp())) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k429TooManyRequests);
            resp->setBody("Rate limit exceeded");
            callback(resp);
            return;
        }

        try {
            std::string email = req->getParameter("email");
            std::string message = req->getParameter("message");
            
            if (email.empty() || message.empty()) {
                throw std::invalid_argument("Email and message are required");
            }
            
            if (!isValidEmail(email)) {
                throw std::invalid_argument("Invalid email format");
            }
            
            if (message.length() > 1000) {
                throw std::invalid_argument("Message too long");
            }
            
            email = sanitizeInput(email);
            message = sanitizeInput(message);
            
            LOG_INFO << "Message submitted - Email: " << email;
            
            std::string html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Message Submitted</title>
    <link rel="stylesheet" href="/static/style.css">
</head>
<body>
    <div class="container">
        <h1>Message Submitted</h1>
        <div class="result">
            <p><strong>Email:</strong> )" + email + R"(</p>
            <p><strong>Message:</strong> )" + message + R"(</p>
        </div>
        <a href="/submit" class="button">Submit Another</a>
        <a href="/" class="link">Back to Home</a>
    </div>
</body>
</html>)";
            
            auto resp = HttpResponse::newHttpResponse();
            resp->setContentTypeCode(CT_TEXT_HTML);
            resp->setBody(html);
            callback(resp);
            
        } catch (const std::exception& e) {
            LOG_ERROR << "Submission error: " << e.what();
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k400BadRequest);
            resp->setBody("Invalid input");
            callback(resp);
        }
    }
};

int main() {
    app().setLogPath("./logs")
         .setLogLevel(trantor::Logger::kInfo)
         .addListener("0.0.0.0", 8080)
         .setThreadNum(4)
         .setDocumentRoot("./static")
         .run();
    
    return 0;
}
