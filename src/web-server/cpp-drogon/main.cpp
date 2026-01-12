#include <drogon/drogon.h>
using namespace drogon;

int main() {
    // Home page
    app().registerHandler("/", [](const HttpRequestPtr& req,
                                 std::function<void(const HttpResponsePtr&)>&& callback) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setContentTypeCode(CT_TEXT_HTML);
        resp->setBody(R"(
<!DOCTYPE html>
<html>
<head>
    <title>Home</title>
    <link rel="stylesheet" href="/style.css">
</head>
<body>
    <h1>Hello. Welcome to the Home Page.</h1>
    <a href="/submit">Submit Message</a>
</body>
</html>
        )");
        callback(resp);
    }, {Get});

    // Submission page
    app().registerHandler("/submit", [](const HttpRequestPtr& req,
                                       std::function<void(const HttpResponsePtr&)>&& callback) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setContentTypeCode(CT_TEXT_HTML);
        resp->setBody(R"(
<!DOCTYPE html>
<html>
<head>
    <title>Submit Message</title>
    <link rel="stylesheet" href="/style.css">
</head>
<body>
    <h1>Submit Message</h1>
    <form method="post" action="/process">
        <label>Email: <input type="email" name="email" required></label><br><br>
        <label>Message: <textarea name="message" required></textarea></label><br><br>
        <button type="submit">Submit</button>
    </form>
    <br><a href="/">Home</a>
</body>
</html>
        )");
        callback(resp);
    }, {Get});

    // Process submission
    app().registerHandler("/process", [](const HttpRequestPtr& req,
                                        std::function<void(const HttpResponsePtr&)>&& callback) {
        auto email = req->getParameter("email");
        auto message = req->getParameter("message");
        
        auto resp = HttpResponse::newHttpResponse();
        resp->setContentTypeCode(CT_TEXT_HTML);
        resp->setBody(R"(
<!DOCTYPE html>
<html>
<head>
    <title>Message Received</title>
    <link rel="stylesheet" href="/style.css">
</head>
<body>
    <h1>Message Received</h1>
    <p><strong>Email:</strong> )" + email + R"(</p>
    <p><strong>Message:</strong> )" + message + R"(</p>
    <a href="/submit">Submit Another Message</a><br>
    <a href="/">Home</a>
</body>
</html>
        )");
        callback(resp);
    }, {Post});

    // CSS stylesheet
    app().registerHandler("/style.css", [](const HttpRequestPtr& req,
                                          std::function<void(const HttpResponsePtr&)>&& callback) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setContentTypeCode(CT_TEXT_CSS);
        resp->setBody(R"(
body { font-family: Arial, sans-serif; margin: 40px; }
h1 { color: #333; }
form { margin: 20px 0; }
label { display: block; margin: 10px 0; }
input, textarea { padding: 8px; margin-left: 10px; }
button { padding: 10px 20px; background: #007cba; color: white; border: none; cursor: pointer; }
a { color: #007cba; text-decoration: none; margin-right: 20px; }
        )");
        callback(resp);
    }, {Get});

    app().setLogPath("./")
        .setLogLevel(trantor::Logger::kWarn)
        .addListener("0.0.0.0", 8080)
        .run();
}
