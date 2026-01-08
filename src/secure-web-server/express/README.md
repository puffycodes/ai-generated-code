# Secure Express Web Server

A minimal Express.js web server with security features.

## Features

- Home page with welcome message
- Message submission form with email and message fields
- Input validation and sanitization
- Security headers (Helmet.js)
- Rate limiting
- Session management
- Logging
- Error handling
- CSS styling

## Security Measures

- XSS prevention through input escaping
- Rate limiting to prevent abuse
- Security headers via Helmet
- Input validation and sanitization
- Proper error handling without sensitive info exposure
- Session management with secure cookies
- Logging for monitoring

## Setup

1. Install dependencies:
   ```bash
   npm install
   ```

2. Set environment variables (optional):
   ```bash
   export SESSION_SECRET="your-secret-key"
   export PORT=3000
   ```

3. Run the server:
   ```bash
   npm start
   ```

   For development:
   ```bash
   npm run dev
   ```

## Usage

- Visit `http://localhost:3000` for the home page
- Click "Submit Message" to access the form
- Submit email and message to see the confirmation page
