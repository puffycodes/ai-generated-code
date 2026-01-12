#include "crow.h"
#include <string>
#include <regex>
#include <unordered_map>
#include <chrono>
#include <mutex>

class RateLimiter {
private:
    std::unordered_map<std::string, std::vector<std::chrono::steady_clock::time_point>> requests;
    std::mutex mtx;
    const int max_requests = 10;
    const std::chrono::seconds window{60};

public:
    bool allow(const std::string& ip) {
        std::lock_guard<std::mutex> lock(mtx);
        auto now = std::chrono::steady_clock::now();
        auto& req_times = requests[ip];
        
        req_times.erase(std::remove_if(req_times.begin(), req_times.end(),
            [&](const auto& time) { return now - time > window; }), req_times.end());
        
        if (req_times.size() >= max_requests) return false;
        req_times.push_back(now);
        return true;
    }
};

std::string sanitize_html(const std::string& input) {
    std::string output = input;
    std::regex html_chars(R"([<>&"'])");
    std::unordered_map<char, std::string> replacements = {
        {'<', "&lt;"}, {'>', "&gt;"}, {'&', "&amp;"}, {'"', "&quot;"}, {'\'', "&#x27;"}
    };
    
    for (auto& pair : replacements) {
        size_t pos = 0;
        std::string search(1, pair.first);
        while ((pos = output.find(search, pos)) != std::string::npos) {
            output.replace(pos, 1, pair.second);
            pos += pair.second.length();
        }
    }
    return output;
}

bool validate_email(const std::string& email) {
    std::regex email_regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(email, email_regex) && email.length() <= 254;
}

std::string get_css() {
    return R"(
body { font-family: Arial, sans-serif; max-width: 800px; margin: 0 auto; padding: 20px; }
.container { background: #f9f9f9; padding: 20px; border-radius: 8px; }
.form-group { margin-bottom: 15px; }
label { display: block; margin-bottom: 5px; font-weight: bold; }
input, textarea { width: 100%; padding: 8px; border: 1px solid #ddd; border-radius: 4px; }
button { background: #007bff; color: white; padding: 10px 20px; border: none; border-radius: 4px; cursor: pointer; }
button:hover { background: #0056b3; }
a { color: #007bff; text-decoration: none; }
a:hover { text-decoration: underline; }
.error { color: red; margin-top: 10px; }
)";
}

int main() {
    crow::SimpleApp app;
    RateLimiter limiter;

    // Middleware for rate limiting
    auto rate_limit = [&limiter](const crow::request& req, crow::response& res, std::function<void()> next) {
        std::string ip = req.get_header_value("X-Forwarded-For");
        if (ip.empty()) ip = req.remote_ip_address;
        
        if (!limiter.allow(ip)) {
            res.code = 429;
            res.write("Rate limit exceeded");
            res.end();
            return;
        }
        next();
    };

    // CSS endpoint
    CROW_ROUTE(app, "/style.css")
    ([](const crow::request& req) {
        auto res = crow::response(200, get_css());
        res.set_header("Content-Type", "text/css");
        return res;
    });

    // Home page
    CROW_ROUTE(app, "/")
    ([rate_limit](const crow::request& req) {
        return crow::response(200, R"(
<!DOCTYPE html>
<html>
<head>
    <title>Home</title>
    <link rel="stylesheet" href="/style.css">
</head>
<body>
    <div class="container">
        <h1>Hello. Welcome to the Home Page.</h1>
        <a href="/submit">Submit Message</a>
    </div>
</body>
</html>
)");
    });

    // Submission page
    CROW_ROUTE(app, "/submit")
    ([rate_limit](const crow::request& req) {
        return crow::response(200, R"(
<!DOCTYPE html>
<html>
<head>
    <title>Submit Message</title>
    <link rel="stylesheet" href="/style.css">
</head>
<body>
    <div class="container">
        <h1>Submit Message</h1>
        <form method="POST" action="/submit">
            <div class="form-group">
                <label for="email">Email:</label>
                <input type="email" id="email" name="email" required maxlength="254">
            </div>
            <div class="form-group">
                <label for="message">Message:</label>
                <textarea id="message" name="message" required maxlength="1000" rows="5"></textarea>
            </div>
            <button type="submit">Submit</button>
        </form>
        <br><a href="/">Back to Home</a>
    </div>
</body>
</html>
)");
    });

    // Handle form submission
    CROW_ROUTE(app, "/submit").methods("POST"_method)
    ([rate_limit](const crow::request& req) {
        std::string email = req.get_body_param("email");
        std::string message = req.get_body_param("message");

        // Input validation
        if (email.empty() || message.empty()) {
            return crow::response(400, R"(
<!DOCTYPE html>
<html>
<head>
    <title>Error</title>
    <link rel="stylesheet" href="/style.css">
</head>
<body>
    <div class="container">
        <h1>Error</h1>
        <p class="error">Email and message are required.</p>
        <a href="/submit">Back to Submission</a>
    </div>
</body>
</html>
)");
        }

        if (!validate_email(email)) {
            return crow::response(400, R"(
<!DOCTYPE html>
<html>
<head>
    <title>Error</title>
    <link rel="stylesheet" href="/style.css">
</head>
<body>
    <div class="container">
        <h1>Error</h1>
        <p class="error">Invalid email format.</p>
        <a href="/submit">Back to Submission</a>
    </div>
</body>
</html>
)");
        }

        // Sanitize output
        std::string safe_email = sanitize_html(email);
        std::string safe_message = sanitize_html(message);

        // Log the submission (basic logging)
        CROW_LOG_INFO << "Message submitted from: " << safe_email;

        return crow::response(200, 
            "<!DOCTYPE html>\n"
            "<html>\n"
            "<head>\n"
            "    <title>Message Received</title>\n"
            "    <link rel=\"stylesheet\" href=\"/style.css\">\n"
            "</head>\n"
            "<body>\n"
            "    <div class=\"container\">\n"
            "        <h1>Message Received</h1>\n"
            "        <p><strong>Email:</strong> " + safe_email + "</p>\n"
            "        <p><strong>Message:</strong> " + safe_message + "</p>\n"
            "        <a href=\"/submit\">Submit Another Message</a> | \n"
            "        <a href=\"/\">Back to Home</a>\n"
            "    </div>\n"
            "</body>\n"
            "</html>"
        );
    });

    app.port(8080).multithreaded().run();
    return 0;
}
