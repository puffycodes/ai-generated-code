# Secure Web Server (Node.js)

A minimal secure web server built with Node.js and Express.

## Features

- Home page with welcome message
- Message submission form with email and message fields
- Input validation and sanitization
- Security headers via Helmet
- Rate limiting
- Session management
- Logging with Winston
- XSS protection through input escaping
- Error handling without sensitive information exposure

## Setup

1. Install dependencies:
   ```bash
   npm install
   ```

2. Set environment variables (optional):
   ```bash
   export SESSION_SECRET="your-secure-session-secret"
   export NODE_ENV="production"
   export PORT="3000"
   ```

3. Run the server:
   ```bash
   npm start
   ```

   For development with auto-reload:
   ```bash
   npm run dev
   ```

## Security Features

- **XSS Prevention**: Input validation and output escaping
- **Rate Limiting**: 100 requests per 15 minutes per IP
- **Security Headers**: Helmet middleware for various security headers
- **Input Validation**: Email and message validation with express-validator
- **Session Security**: Secure session configuration
- **Logging**: Request and error logging without exposing sensitive data
- **Error Handling**: Generic error messages to prevent information disclosure

## Usage

1. Visit `http://localhost:3000` for the home page
2. Click "Submit Message" to access the submission form
3. Fill in email and message (max 500 characters)
4. Submit to see the confirmation page
