# Secure Web Server - Spring Boot

A secure web server implementation using Spring Boot framework with comprehensive security measures.

## Features

- **Home Page**: Welcome page with navigation to message submission
- **Message Submission**: Form for email and message input with validation
- **Confirmation Page**: Displays submitted message details
- **Security**: Comprehensive security implementation including XSS protection, input validation, and secure headers

## Security Measures Implemented

1. **SQL Injection Prevention**: Uses JPA with parameterized queries
2. **XSS Protection**: Thymeleaf templates with automatic escaping
3. **Input Validation**: Bean validation with custom constraints
4. **Output Sanitization**: Safe rendering through Thymeleaf
5. **Security Headers**: HSTS, Content-Type options, Frame options
6. **Error Handling**: Secure error messages without sensitive information exposure
7. **Logging**: Comprehensive logging for security monitoring
8. **Session Security**: HTTP-only and secure cookie flags

## Running the Application

1. Navigate to the project directory:
   ```bash
   cd ../ai-generated-code/src/secure-web-server/spring-boot/
   ```

2. Run with Maven:
   ```bash
   ./mvnw spring-boot:run
   ```

3. Access the application at: http://localhost:8080

## Project Structure

```
src/
├── main/
│   ├── java/com/example/securewebserver/
│   │   ├── SecureWebServerApplication.java
│   │   ├── config/SecurityConfig.java
│   │   ├── controller/WebController.java
│   │   ├── model/Message.java
│   │   └── repository/MessageRepository.java
│   └── resources/
│       ├── static/css/style.css
│       ├── templates/
│       │   ├── home.html
│       │   ├── submit.html
│       │   └── confirmation.html
│       └── application.properties
├── pom.xml
└── .gitignore
```

## Dependencies

- Spring Boot Web
- Spring Boot Security
- Spring Boot Validation
- Spring Boot Data JPA
- Thymeleaf
- H2 Database (in-memory)
