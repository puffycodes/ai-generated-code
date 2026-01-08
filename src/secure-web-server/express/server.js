const express = require('express');
const helmet = require('helmet');
const rateLimit = require('express-rate-limit');
const session = require('express-session');
const { body, validationResult } = require('express-validator');
const winston = require('winston');
const path = require('path');

const app = express();
const PORT = process.env.PORT || 3000;

// Logger setup
const logger = winston.createLogger({
  level: 'info',
  format: winston.format.combine(
    winston.format.timestamp(),
    winston.format.json()
  ),
  transports: [
    new winston.transports.File({ filename: 'app.log' }),
    new winston.transports.Console()
  ]
});

// Security middleware
app.use(helmet());
app.use(express.json({ limit: '10mb' }));
app.use(express.urlencoded({ extended: true, limit: '10mb' }));

// Rate limiting
const limiter = rateLimit({
  windowMs: 15 * 60 * 1000, // 15 minutes
  max: 100 // limit each IP to 100 requests per windowMs
});
app.use(limiter);

// Session management
app.use(session({
  secret: process.env.SESSION_SECRET || 'change-this-in-production',
  resave: false,
  saveUninitialized: false,
  cookie: { secure: false, httpOnly: true, maxAge: 3600000 } // 1 hour
}));

// Static files
app.use(express.static('public'));

// Input validation middleware
const validateMessage = [
  body('email').isEmail().normalizeEmail().escape(),
  body('message').trim().isLength({ min: 1, max: 500 }).escape()
];

// Routes
app.get('/', (req, res) => {
  logger.info('Home page accessed');
  res.send(`
    <!DOCTYPE html>
    <html>
    <head>
      <title>Secure Web Server</title>
      <link rel="stylesheet" href="/styles.css">
    </head>
    <body>
      <div class="container">
        <h1>Hello. Welcome to the Home Page.</h1>
        <a href="/submit" class="button">Submit Message</a>
      </div>
    </body>
    </html>
  `);
});

app.get('/submit', (req, res) => {
  logger.info('Submit page accessed');
  res.send(`
    <!DOCTYPE html>
    <html>
    <head>
      <title>Submit Message</title>
      <link rel="stylesheet" href="/styles.css">
    </head>
    <body>
      <div class="container">
        <h1>Submit Message</h1>
        <form method="POST" action="/submit">
          <div class="form-group">
            <label for="email">Email:</label>
            <input type="email" id="email" name="email" required>
          </div>
          <div class="form-group">
            <label for="message">Message:</label>
            <textarea id="message" name="message" required maxlength="500"></textarea>
          </div>
          <button type="submit" class="button">Submit</button>
        </form>
        <a href="/" class="link">Back to Home</a>
      </div>
    </body>
    </html>
  `);
});

app.post('/submit', validateMessage, (req, res) => {
  const errors = validationResult(req);
  if (!errors.isEmpty()) {
    logger.warn('Validation errors:', errors.array());
    return res.status(400).send(`
      <!DOCTYPE html>
      <html>
      <head>
        <title>Error</title>
        <link rel="stylesheet" href="/styles.css">
      </head>
      <body>
        <div class="container">
          <h1>Validation Error</h1>
          <p>Please check your input and try again.</p>
          <a href="/submit" class="button">Back to Submit</a>
          <a href="/" class="link">Back to Home</a>
        </div>
      </body>
      </html>
    `);
  }

  const { email, message } = req.body;
  logger.info('Message submitted', { email: email.substring(0, 3) + '***' });

  res.send(`
    <!DOCTYPE html>
    <html>
    <head>
      <title>Message Received</title>
      <link rel="stylesheet" href="/styles.css">
    </head>
    <body>
      <div class="container">
        <h1>Message Received</h1>
        <div class="message-display">
          <p><strong>Email:</strong> ${email}</p>
          <p><strong>Message:</strong> ${message}</p>
        </div>
        <a href="/submit" class="button">Submit Another</a>
        <a href="/" class="link">Back to Home</a>
      </div>
    </body>
    </html>
  `);
});

// Error handling
app.use((err, req, res, next) => {
  logger.error('Server error:', err.message);
  res.status(500).send(`
    <!DOCTYPE html>
    <html>
    <head>
      <title>Server Error</title>
      <link rel="stylesheet" href="/styles.css">
    </head>
    <body>
      <div class="container">
        <h1>Server Error</h1>
        <p>Something went wrong. Please try again later.</p>
        <a href="/" class="button">Back to Home</a>
      </div>
    </body>
    </html>
  `);
});

app.listen(PORT, () => {
  logger.info(`Server running on port ${PORT}`);
});
