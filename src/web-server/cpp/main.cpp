#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

class WebServer {
private:
    int port;
    int server_fd;
    
    std::string url_decode(const std::string& str) {
        std::string result;
        for (size_t i = 0; i < str.length(); ++i) {
            if (str[i] == '%' && i + 2 < str.length()) {
                int hex = std::stoi(str.substr(i + 1, 2), nullptr, 16);
                result += static_cast<char>(hex);
                i += 2;
            } else if (str[i] == '+') {
                result += ' ';
            } else {
                result += str[i];
            }
        }
        return result;
    }
    
    std::map<std::string, std::string> parse_form_data(const std::string& data) {
        std::map<std::string, std::string> params;
        std::istringstream stream(data);
        std::string pair;
        
        while (std::getline(stream, pair, '&')) {
            size_t pos = pair.find('=');
            if (pos != std::string::npos) {
                std::string key = url_decode(pair.substr(0, pos));
                std::string value = url_decode(pair.substr(pos + 1));
                params[key] = value;
            }
        }
        return params;
    }
    
    std::string get_home_page() {
        return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
               "<!DOCTYPE html><html><head><title>Home</title><link rel='stylesheet' href='/style.css'></head>"
               "<body><h1>Hello. Welcome to the Home Page.</h1>"
               "<a href='/submit'>Submit Message</a></body></html>";
    }
    
    std::string get_submit_page() {
        return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
               "<!DOCTYPE html><html><head><title>Submit</title><link rel='stylesheet' href='/style.css'></head>"
               "<body><h1>Submit Message</h1>"
               "<form method='POST' action='/submit'>"
               "<label>Email: <input type='email' name='email' required></label><br><br>"
               "<label>Message: <textarea name='message' required></textarea></label><br><br>"
               "<input type='submit' value='Submit'></form>"
               "<a href='/'>Home</a></body></html>";
    }
    
    std::string get_result_page(const std::string& email, const std::string& message) {
        return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
               "<!DOCTYPE html><html><head><title>Result</title><link rel='stylesheet' href='/style.css'></head>"
               "<body><h1>Message Submitted</h1>"
               "<p><strong>Email:</strong> " + email + "</p>"
               "<p><strong>Message:</strong> " + message + "</p>"
               "<a href='/submit'>Submit Another</a> | <a href='/'>Home</a></body></html>";
    }
    
    std::string get_css() {
        return "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\n\r\n"
               "body { font-family: Arial, sans-serif; margin: 40px; }"
               "h1 { color: #333; }"
               "form { margin: 20px 0; }"
               "label { display: block; margin: 10px 0; }"
               "input, textarea { padding: 8px; margin: 5px 0; }"
               "textarea { width: 300px; height: 100px; }"
               "a { color: #007bff; text-decoration: none; margin: 0 10px; }"
               "a:hover { text-decoration: underline; }";
    }
    
public:
    WebServer(int p) : port(p) {}
    
    void start() {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        int opt = 1;
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        
        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);
        
        bind(server_fd, (struct sockaddr*)&address, sizeof(address));
        listen(server_fd, 3);
        
        std::cout << "Server running on http://localhost:" << port << std::endl;
        
        while (true) {
            int client_socket = accept(server_fd, nullptr, nullptr);
            std::thread(&WebServer::handle_request, this, client_socket).detach();
        }
    }
    
    void handle_request(int client_socket) {
        char buffer[4096] = {0};
        read(client_socket, buffer, 4096);
        
        std::string request(buffer);
        std::istringstream stream(request);
        std::string method, path;
        stream >> method >> path;
        
        std::string response;
        
        if (method == "GET") {
            if (path == "/" || path == "/home") {
                response = get_home_page();
            } else if (path == "/submit") {
                response = get_submit_page();
            } else if (path == "/style.css") {
                response = get_css();
            }
        } else if (method == "POST" && path == "/submit") {
            size_t body_start = request.find("\r\n\r\n");
            if (body_start != std::string::npos) {
                std::string body = request.substr(body_start + 4);
                auto params = parse_form_data(body);
                response = get_result_page(params["email"], params["message"]);
            }
        }
        
        if (response.empty()) {
            response = "HTTP/1.1 404 Not Found\r\n\r\n404 Not Found";
        }
        
        send(client_socket, response.c_str(), response.length(), 0);
        close(client_socket);
    }
};

int main() {
    WebServer server(8080);
    server.start();
    return 0;
}
