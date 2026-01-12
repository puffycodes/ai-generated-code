# Secure Web Server (Go)

A secure web server implementation with the following features:

## Security Features

- **CSRF Protection**: Uses Gorilla CSRF middleware
- **XSS Prevention**: HTML escaping of user inputs
- **Rate Limiting**: 10 requests per second per client
- **Input Validation**: Required field validation
- **Session Management**: Secure cookie-based sessions
- **No Hardcoded Secrets**: Uses environment variables

## Setup

1. Install dependencies:
```bash
go mod tidy
```

2. Set required environment variable:
```bash
export SESSION_KEY="your-32-character-secret-key-here"
```

3. Run the server:
```bash
go run main.go
```

4. Visit http://localhost:8080

## Project Structure

```
.
├── main.go           # Main server code
├── go.mod           # Go module file
├── templates/       # HTML templates
│   ├── home.html
│   ├── submit.html
│   └── result.html
├── static/          # Static assets
│   └── style.css
└── README.md        # This file
```

## Usage

- Home page: Welcome message with link to submission form
- Submit page: Form for email and message input
- Result page: Displays submitted data with navigation links
