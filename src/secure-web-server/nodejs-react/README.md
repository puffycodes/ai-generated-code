# Secure Web Server - Node.js & React

A secure web server implementation with Node.js backend and React frontend, featuring comprehensive security measures.

## Features

- **Home Page**: Welcome message with navigation to submission form
- **Submission Form**: Email and message input with validation
- **Result Display**: Shows submitted data after successful submission
- **Security Features**:
  - SQL injection prevention (parameterized queries)
  - XSS protection (input sanitization)
  - Rate limiting
  - Session management
  - Input validation
  - Security headers (Helmet.js)
  - Logging system
  - CORS protection

## Setup

1. Install dependencies:
   ```bash
   npm install
   cd client && npm install
   ```

2. Create logs directory:
   ```bash
   mkdir logs
   ```

3. Development mode:
   ```bash
   npm run dev
   ```

4. Production build:
   ```bash
   cd client && npm run build
   cd .. && npm start
   ```

## Environment Variables

- `NODE_ENV`: Set to 'production' for production deployment
- `SESSION_SECRET`: Secure session secret (required for production)
- `PORT`: Server port (default: 3001)

## Security Measures Implemented

- **Helmet.js**: Security headers including CSP
- **Rate Limiting**: 100 requests per 15 minutes per IP
- **Input Validation**: Server-side validation with express-validator
- **XSS Protection**: DOMPurify sanitization
- **SQL Injection Prevention**: Sequelize ORM with parameterized queries
- **Session Security**: Secure cookies, httpOnly flags
- **Error Handling**: No sensitive information exposure
- **Logging**: Winston logger for security events
