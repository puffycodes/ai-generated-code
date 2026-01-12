# Secure Web Server - C++ with Crow Framework

A minimal secure web server implementation using C++ and the Crow framework.

## Features

- Home page with welcome message
- Message submission form with email validation
- XSS protection through HTML sanitization
- Input validation and length limits
- Rate limiting (10 requests per minute per IP)
- Basic logging
- CSS styling
- Error handling

## Security Measures

- **XSS Prevention**: HTML output sanitization
- **Input Validation**: Email format and length validation
- **Rate Limiting**: IP-based request throttling
- **Error Handling**: Safe error messages without sensitive info
- **Logging**: Basic request logging
- **No Hardcoded Secrets**: No embedded credentials

## Build Requirements

- C++17 compiler
- CMake 3.10+
- Crow framework
- pkg-config

## Installation

### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential cmake pkg-config
sudo apt install libcrow-dev
```

### Build
```bash
mkdir build && cd build
cmake ..
make
```

## Usage

```bash
./secure_web_server
```

Server runs on http://localhost:8080

## Endpoints

- `/` - Home page
- `/submit` - Message submission form (GET/POST)
- `/style.css` - CSS stylesheet
