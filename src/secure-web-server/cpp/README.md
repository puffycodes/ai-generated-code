# Secure Web Server (C++)

A minimal secure web server implementation in C++ using libmicrohttpd.

## Features

- **Home Page**: Welcome message with navigation
- **Message Submission**: Form for email and message input
- **Result Display**: Shows submitted data with proper escaping
- **Security Features**:
  - XSS prevention through HTML escaping
  - Input validation (email format, message length)
  - Rate limiting (60 requests/minute per IP)
  - Security headers (X-Frame-Options, X-XSS-Protection, etc.)
  - Request logging
  - No hardcoded secrets
  - Proper error handling

## Dependencies

- libmicrohttpd (HTTP server library)
- CMake 3.10+
- C++17 compiler

## Build Instructions

### Ubuntu/Debian
```bash
sudo apt-get install libmicrohttpd-dev cmake build-essential
mkdir build && cd build
cmake ..
make
```

### CentOS/RHEL
```bash
sudo yum install libmicrohttpd-devel cmake gcc-c++
mkdir build && cd build
cmake ..
make
```

## Running

```bash
./secure_web_server
```

Server will start on http://localhost:8080

## Security Measures

1. **XSS Prevention**: All user input is HTML-escaped before display
2. **Input Validation**: Email format and message length validation
3. **Rate Limiting**: Prevents abuse with 60 requests/minute limit
4. **Security Headers**: Adds protective HTTP headers
5. **Logging**: All requests are logged with timestamps
6. **Error Handling**: Safe error messages without sensitive info exposure
7. **Memory Safety**: Proper resource management and bounds checking
