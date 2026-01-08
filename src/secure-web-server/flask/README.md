# Secure Flask Web Server

A minimal Flask web server with security features implemented.

## Setup

1. Install dependencies:
```bash
pip install -r requirements.txt
```

2. Set environment variable for production:
```bash
export SECRET_KEY="your-secret-key-here"
```

3. Run the server:
```bash
python app.py
```

## Features

- Home page with welcome message
- Message submission form with email validation
- Result display page
- CSRF protection
- Rate limiting
- Input validation and sanitization
- Session management
- Logging
- Error handling

## Security Features

- CSRF protection using Flask-WTF
- Rate limiting to prevent abuse
- Input validation and output escaping
- Secure session management
- Proper error handling without information disclosure
- Logging for security monitoring
- No hardcoded secrets (uses environment variables)

## Usage

1. Visit http://127.0.0.1:5000 for the home page
2. Click "Submit Message" to access the form
3. Enter email and message, then submit
4. View the confirmation page with your submitted data
