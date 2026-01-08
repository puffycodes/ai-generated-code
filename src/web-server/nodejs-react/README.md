# Node.js React Web Server

A minimal web server built with Node.js and React that implements the specified features.

## Features

- Home page with welcome message and navigation
- Message submission form with email and message fields
- Result page displaying submitted data
- CSS styling for all pages
- Navigation between pages

## Setup

1. Install dependencies:
   ```bash
   npm install
   cd client && npm install
   ```

2. Run in development mode:
   ```bash
   npm run dev
   ```

3. Build for production:
   ```bash
   npm run build
   npm start
   ```

## Structure

- `server/` - Express.js backend
- `client/` - React frontend
- API endpoint: `POST /api/submit` for message submission
