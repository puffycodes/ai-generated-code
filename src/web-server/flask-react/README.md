# Flask-React Web Server

A minimal web server built with Flask backend and React frontend.

## Setup

### Backend (Flask)
```bash
pip install -r requirements.txt
python app.py
```

### Frontend (React)
```bash
npm install
npm run build
```

## Usage

1. Start Flask server: `python app.py`
2. Build React app: `npm run build`
3. Visit http://localhost:5000

## Development

For development with hot reload:
1. Start Flask: `python app.py`
2. Start React dev server: `npm start` (runs on port 3000)

## Features

- Home page with welcome message
- Submission form for email and message
- Result page displaying submitted data
- CSS styling
- Navigation between pages
