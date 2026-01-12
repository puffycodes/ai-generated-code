#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <regex>
#include <chrono>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <ctime>
#include <fstream>
#include <microhttpd.h>

class Logger {
public:
    static void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(log_mutex);
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::cout << "[" << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") << "] " << message << std::endl;
    }
private:
    static std::mutex log_mutex;
};
std::mutex Logger::log_mutex;

class RateLimiter {
public:
    bool isAllowed(const std::string& ip) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto now = std::chrono::steady_clock::now();
        auto& entry = requests_[ip];
        
        // Clean old entries
        while (!entry.empty() && now - entry.front() > std::chrono::minutes(1)) {
            entry.pop_front();
        }
        
        if (entry.size() >= 60) { // 60 requests per minute
            return false;
        }
        
        entry.push_back(now);
        return true;
    }
    
private:
    std::mutex mutex_;
    std::unordered_map<std::string, std::deque<std::chrono::steady_clock::time_point>> requests_;
};

class SecurityUtils {
public:
    static std::string escapeHtml(const std::string& input) {
        std::string output;
        for (char c : input) {
            switch (c) {
                case '<': output += "&lt;"; break;
                case '>': output += "&gt;"; break;
                case '&': output += "&amp;"; break;
                case '"': output += "&quot;"; break;
                case '\'': output += "&#x27;"; break;
                default: output += c; break;
            }
        }
        return output;
    }
    
    static bool validateEmail(const std::string& email) {
        std::regex email_regex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        return std::regex_match(email, email_regex) && email.length() <= 254;
    }
    
    static bool validateMessage(const std::string& message) {
        return message.length() <= 1000 && !message.empty();
    }
};

class WebServer {
private:
    RateLimiter rateLimiter;
    
    static std::string loadCSS() {
        return R"(
body { font-family: Arial, sans-serif; max-width: 800px; margin: 0 auto; padding: 20px; }
.container { background: #f9f9f9; padding: 20px; border-radius: 8px; }
.form-group { margin-bottom: 15px; }
label { display: block; margin-bottom: 5px; font-weight: bold; }
input, textarea { width: 100%; padding: 8px; border: 1px solid #ddd; border-radius: 4px; }
button { background: #007bff; color: white; padding: 10px 20px; border: none; border-radius: 4px; cursor: pointer; }
button:hover { background: #0056b3; }
.nav-link { color: #007bff; text-decoration: none; margin-right: 15px; }
.nav-link:hover { text-decoration: underline; }
.error { color: #dc3545; background: #f8d7da; padding: 10px; border-radius: 4px; margin: 10px 0; }
)";
    }
    
    static std::string getHomePage() {
        return R"(<!DOCTYPE html>
<html><head><title>Home</title><style>)" + loadCSS() + R"(</style></head>
<body><div class="container">
<h1>Hello. Welcome to the Home Page.</h1>
<a href="/submit" class="nav-link">Submit Message</a>
</div></body></html>)";
    }
    
    static std::string getSubmitPage(const std::string& error = "") {
        std::string errorHtml = error.empty() ? "" : "<div class=\"error\">" + SecurityUtils::escapeHtml(error) + "</div>";
        return R"(<!DOCTYPE html>
<html><head><title>Submit Message</title><style>)" + loadCSS() + R"(</style></head>
<body><div class="container">
<a href="/" class="nav-link">Home</a>
<h1>Submit Message</h1>)" + errorHtml + R"(
<form method="post" action="/submit">
<div class="form-group">
<label for="email">Email:</label>
<input type="email" id="email" name="email" required>
</div>
<div class="form-group">
<label for="message">Message:</label>
<textarea id="message" name="message" rows="4" required></textarea>
</div>
<button type="submit">Submit</button>
</form>
</div></body></html>)";
    }
    
    static std::string getResultPage(const std::string& email, const std::string& message) {
        return R"(<!DOCTYPE html>
<html><head><title>Message Submitted</title><style>)" + loadCSS() + R"(</style></head>
<body><div class="container">
<a href="/" class="nav-link">Home</a>
<h1>Message Submitted</h1>
<p><strong>Email:</strong> )" + SecurityUtils::escapeHtml(email) + R"(</p>
<p><strong>Message:</strong> )" + SecurityUtils::escapeHtml(message) + R"(</p>
<a href="/submit" class="nav-link">Submit Another Message</a>
</div></body></html>)";
    }
    
    static std::map<std::string, std::string> parseFormData(const std::string& data) {
        std::map<std::string, std::string> result;
        std::istringstream stream(data);
        std::string pair;
        
        while (std::getline(stream, pair, '&')) {
            size_t pos = pair.find('=');
            if (pos != std::string::npos) {
                std::string key = pair.substr(0, pos);
                std::string value = pair.substr(pos + 1);
                
                // URL decode
                std::string decoded;
                for (size_t i = 0; i < value.length(); ++i) {
                    if (value[i] == '%' && i + 2 < value.length()) {
                        int hex = std::stoi(value.substr(i + 1, 2), nullptr, 16);
                        decoded += static_cast<char>(hex);
                        i += 2;
                    } else if (value[i] == '+') {
                        decoded += ' ';
                    } else {
                        decoded += value[i];
                    }
                }
                result[key] = decoded;
            }
        }
        return result;
    }
    
public:
    static int handleRequest(void* cls, struct MHD_Connection* connection,
                           const char* url, const char* method,
                           const char* version, const char* upload_data,
                           size_t* upload_data_size, void** con_cls) {
        
        WebServer* server = static_cast<WebServer*>(cls);
        
        // Get client IP for rate limiting
        const char* client_ip = MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "X-Forwarded-For");
        if (!client_ip) client_ip = "unknown";
        
        if (!server->rateLimiter.isAllowed(client_ip)) {
            Logger::log("Rate limit exceeded for IP: " + std::string(client_ip));
            const char* error_page = "<html><body><h1>429 Too Many Requests</h1></body></html>";
            struct MHD_Response* response = MHD_create_response_from_buffer(strlen(error_page), (void*)error_page, MHD_RESPMEM_PERSISTENT);
            int ret = MHD_queue_response(connection, 429, response);
            MHD_destroy_response(response);
            return ret;
        }
        
        Logger::log(std::string(method) + " " + std::string(url) + " from " + std::string(client_ip));
        
        std::string path(url);
        
        if (strcmp(method, "GET") == 0) {
            std::string response_str;
            
            if (path == "/" || path == "/home") {
                response_str = getHomePage();
            } else if (path == "/submit") {
                response_str = getSubmitPage();
            } else {
                response_str = "<html><body><h1>404 Not Found</h1><a href=\"/\">Home</a></body></html>";
                struct MHD_Response* response = MHD_create_response_from_buffer(response_str.length(), (void*)response_str.c_str(), MHD_RESPMEM_MUST_COPY);
                MHD_add_response_header(response, "Content-Type", "text/html");
                int ret = MHD_queue_response(connection, 404, response);
                MHD_destroy_response(response);
                return ret;
            }
            
            struct MHD_Response* response = MHD_create_response_from_buffer(response_str.length(), (void*)response_str.c_str(), MHD_RESPMEM_MUST_COPY);
            MHD_add_response_header(response, "Content-Type", "text/html");
            MHD_add_response_header(response, "X-Content-Type-Options", "nosniff");
            MHD_add_response_header(response, "X-Frame-Options", "DENY");
            MHD_add_response_header(response, "X-XSS-Protection", "1; mode=block");
            int ret = MHD_queue_response(connection, 200, response);
            MHD_destroy_response(response);
            return ret;
        }
        
        if (strcmp(method, "POST") == 0 && path == "/submit") {
            if (*con_cls == nullptr) {
                *con_cls = (void*)1;
                return MHD_YES;
            }
            
            if (*upload_data_size != 0) {
                std::string* post_data = static_cast<std::string*>(*con_cls);
                if (post_data == (void*)1) {
                    post_data = new std::string();
                    *con_cls = post_data;
                }
                post_data->append(upload_data, *upload_data_size);
                *upload_data_size = 0;
                return MHD_YES;
            }
            
            std::string response_str;
            std::string* post_data = static_cast<std::string*>(*con_cls);
            
            if (post_data && post_data != (void*)1) {
                auto form_data = parseFormData(*post_data);
                std::string email = form_data["email"];
                std::string message = form_data["message"];
                
                if (!SecurityUtils::validateEmail(email)) {
                    response_str = getSubmitPage("Invalid email format");
                } else if (!SecurityUtils::validateMessage(message)) {
                    response_str = getSubmitPage("Message is required and must be less than 1000 characters");
                } else {
                    Logger::log("Message submitted from: " + email);
                    response_str = getResultPage(email, message);
                }
                
                delete post_data;
            } else {
                response_str = getSubmitPage("No data received");
            }
            
            struct MHD_Response* response = MHD_create_response_from_buffer(response_str.length(), (void*)response_str.c_str(), MHD_RESPMEM_MUST_COPY);
            MHD_add_response_header(response, "Content-Type", "text/html");
            MHD_add_response_header(response, "X-Content-Type-Options", "nosniff");
            MHD_add_response_header(response, "X-Frame-Options", "DENY");
            MHD_add_response_header(response, "X-XSS-Protection", "1; mode=block");
            int ret = MHD_queue_response(connection, 200, response);
            MHD_destroy_response(response);
            return ret;
        }
        
        return MHD_NO;
    }
    
    void start(int port = 8080) {
        struct MHD_Daemon* daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, port, nullptr, nullptr,
                                                    &handleRequest, this, MHD_OPTION_END);
        
        if (!daemon) {
            Logger::log("Failed to start server");
            return;
        }
        
        Logger::log("Server started on port " + std::to_string(port));
        Logger::log("Visit http://localhost:" + std::to_string(port));
        
        // Keep server running
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        
        MHD_stop_daemon(daemon);
    }
};

int main() {
    WebServer server;
    server.start(8080);
    return 0;
}
