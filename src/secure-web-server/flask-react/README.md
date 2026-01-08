# Secure Web Server - Flask + React

A secure web application with Flask backend and React frontend.

## Setup

### Backend (Flask)
```bash
pip install -r requirements.txt
export SECRET_KEY=your-secret-key-here
python app.py
```

### Frontend (React)
```bash
npm install
npm run build
```

## Security Features

- Input validation and sanitization
- Rate limiting (10 requests/minute per IP)
- CSRF protection via session tokens
- XSS prevention through HTML escaping
- Security headers via Flask-Talisman
- Proper error handling without information disclosure
- Logging for security monitoring
- No hardcoded secrets (uses environment variables)

## Usage

1. Start Flask server: `python app.py`
2. Build React frontend: `npm run build`
3. Access application at `http://127.0.0.1:5000`

The application serves the React build files through Flask for a single-server deployment.
