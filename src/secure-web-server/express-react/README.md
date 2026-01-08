# Secure Web Server - Express & React

A secure web server implementation using Express.js backend and React frontend.

## Features

- **Home Page**: Welcome message with navigation to submission form
- **Submission Form**: Email and message input with validation
- **Result Display**: Shows submitted data with navigation options
- **Security**: Input validation, XSS protection, rate limiting, CORS, helmet security headers

## Security Measures

- Input validation and sanitization using express-validator
- XSS protection through data escaping
- Rate limiting to prevent abuse
- CORS configuration
- Security headers via Helmet
- Error handling without sensitive information exposure
- Request logging
- No hardcoded secrets (uses environment variables)

## Setup

1. Install dependencies:
   ```bash
   npm install
   cd client && npm install
   ```

2. Copy environment file:
   ```bash
   cp .env.example .env
   ```

3. Run in development:
   ```bash
   npm run dev
   ```

4. Build for production:
   ```bash
   npm run build
   npm start
   ```

## Project Structure

```
├── server/
│   └── index.js          # Express server
├── client/
│   ├── public/
│   │   └── index.html    # HTML template
│   └── src/
│       ├── components/   # React components
│       ├── App.js        # Main app component
│       ├── App.css       # Styles
│       └── index.js      # React entry point
├── package.json          # Root dependencies
└── .gitignore           # Git ignore rules
```
