# Secure Web Server - C++ Drogon

A secure web server implementation using C++ and the Drogon framework.

## Features

- **Home Page**: Welcome message with navigation
- **Message Submission**: Form for email and message input
- **Security Measures**:
  - Input validation and sanitization (XSS prevention)
  - Rate limiting (10 requests per minute per IP)
  - Email format validation
  - HTML entity encoding
  - Error handling without sensitive information exposure
  - Logging mechanism
  - Content length limits

## Prerequisites

- C++17 compiler
- CMake 3.10+
- Drogon framework

## Installation

### Install Drogon

```bash
# Ubuntu/Debian
sudo apt-get install libdrogon-dev

# Or build from source
git clone https://github.com/drogonframework/drogon
cd drogon
git submodule update --init
mkdir build && cd build
cmake ..
make && sudo make install
```

## Build and Run

```bash
mkdir build && cd build
cmake ..
make
./SecureWebServer
```

The server will start on http://localhost:8080

## Project Structure

```
cpp-drogon/
├── main.cpp           # Main application with controllers
├── static/
│   └── style.css      # CSS stylesheet
├── CMakeLists.txt     # Build configuration
├── .gitignore         # Git ignore rules
└── README.md          # This file
```

## Security Features

1. **XSS Prevention**: HTML entity encoding of user inputs
2. **Input Validation**: Email format and length validation
3. **Rate Limiting**: 10 requests per minute per IP address
4. **Error Handling**: Generic error messages without sensitive details
5. **Logging**: Request logging for monitoring
6. **Content Limits**: Maximum message length enforcement
