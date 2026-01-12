#include "crow.h"
#include <string>

int main() {
    crow::SimpleApp app;

    // Home page
    CROW_ROUTE(app, "/")
    ([]{
        return R"(
<!DOCTYPE html>
<html>
<head>
    <title>Home</title>
    <link rel="stylesheet" href="/static/style.css">
</head>
<body>
    <h1>Hello. Welcome to the Home Page.</h1>
    <a href="/submit">Submit Message</a>
</body>
</html>
        )";
    });

    // Submission page
    CROW_ROUTE(app, "/submit")
    ([]{
        return R"(
<!DOCTYPE html>
<html>
<head>
    <title>Submit Message</title>
    <link rel="stylesheet" href="/static/style.css">
</head>
<body>
    <h1>Submit Message</h1>
    <form method="POST" action="/submit">
        <label>Email: <input type="email" name="email" required></label><br>
        <label>Message: <textarea name="message" required></textarea></label><br>
        <button type="submit">Submit</button>
    </form>
    <a href="/">Home</a>
</body>
</html>
        )";
    });

    // Handle form submission
    CROW_ROUTE(app, "/submit").methods("POST"_method)
    ([](const crow::request& req){
        auto email = req.get_body_params().get("email");
        auto message = req.get_body_params().get("message");
        
        return crow::response(200, 
            "<!DOCTYPE html>"
            "<html>"
            "<head>"
            "<title>Message Received</title>"
            "<link rel=\"stylesheet\" href=\"/static/style.css\">"
            "</head>"
            "<body>"
            "<h1>Message Received</h1>"
            "<p><strong>Email:</strong> " + std::string(email ? email : "") + "</p>"
            "<p><strong>Message:</strong> " + std::string(message ? message : "") + "</p>"
            "<a href=\"/submit\">Submit Another Message</a><br>"
            "<a href=\"/\">Home</a>"
            "</body>"
            "</html>"
        );
    });

    // Serve static files
    CROW_ROUTE(app, "/static/<path>")
    ([](const std::string& path){
        return crow::load_text("static/" + path);
    });

    app.port(8080).multithreaded().run();
}
