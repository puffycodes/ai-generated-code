# Express + React Web Server

A minimal web server built with Express.js backend and React frontend.

## Features

- Home page with welcome message
- Message submission form with email and message fields
- Result page displaying submitted data
- CSS styling for all pages
- Navigation between pages

## Setup

1. Install dependencies:
```bash
npm install
```

2. Install client dependencies:
```bash
cd client && npm install
```

## Development

Run both server and client in development mode:
```bash
npm run dev
```

Or run separately:
- Server: `npm run server` (runs on port 5000)
- Client: `npm run client` (runs on port 3000)

## Production

1. Build the React app:
```bash
npm run build
```

2. Start the production server:
```bash
npm start
```

The server will serve the built React app and handle API requests.
