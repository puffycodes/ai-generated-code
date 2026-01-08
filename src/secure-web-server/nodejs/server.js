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
  max: 100, // limit each IP to 100 requests per windowMs
  message: 'Too many requests from this IP'
});
app.use(limiter);

// Session management
app.use(session({
  secret: process.env.SESSION_SECRET || 'fallback-secret-change-in-production',
  resave: false,
  saveUninitialized: false,
  cookie: { 
    secure: process.env.NODE_ENV === 'production',
    httpOnly: true,
    maxAge: 24 * 60 * 60 * 1000 // 24 hours
  }
}));

// Static files
app.use(express.static('public'));

// Input validation middleware
const validateMessage = [
  body('email').isEmail().normalizeEmail().escape(),
  body('message').trim().isLength({ min: 1, max: 500 }).escape()
];

// Error handler
const handleValidationErrors = (req, res, next) => {
  const errors = validationResult(req);
  if (!errors.isEmpty()) {
    logger.warn('Validation errors:', { errors: errors.array(), ip: req.ip });
    return res.status(400).send(`
      <!DOCTYPE html>
      <html>
      <head><title>Error</title><link rel="stylesheet" href="/styles.css"></head>
      <body>
        <h1>Validation Error</h1>
        <p>Please check your input and try again.</p>
        <a href="/">Home</a> | <a href="/submit">Submit Message</a>
      </body>
      </html>
    `);
  }
  next();
};

// Routes
app.get('/', (req, res) => {
  logger.info('Home page accessed', { ip: req.ip });
  res.send(`
    <!DOCTYPE html>
    <html>
    <head><title>Home</title><link rel="stylesheet" href="/styles.css"></head>
    <body>
      <h1>Hello. Welcome to the Home Page.</h1>
      <a href="/submit">Submit Message</a>
    </body>
    </html>
  `);
});

app.get('/submit', (req, res) => {
  logger.info('Submit page accessed', { ip: req.ip });
  res.send(`
    <!DOCTYPE html>
    <html>
    <head><title>Submit Message</title><link rel="stylesheet" href="/styles.css"></head>
    <body>
      <h1>Submit Message</h1>
      <form method="POST" action="/submit">
        <label for="email">Email:</label>
        <input type="email" id="email" name="email" required>
        <label for="message">Message:</label>
        <textarea id="message" name="message" required maxlength="500"></textarea>
        <button type="submit">Submit</button>
      </form>
      <a href="/">Home</a>
    </body>
    </html>
  `);
});

app.post('/submit', validateMessage, handleValidationErrors, (req, res) => {
  const { email, message } = req.body;
  
  logger.info('Message submitted', { 
    email: email.substring(0, 3) + '***', // Log partial email for privacy
    messageLength: message.length,
    ip: req.ip 
  });

  res.send(`
    <!DOCTYPE html>
    <html>
    <head><title>Message Received</title><link rel="stylesheet" href="/styles.css"></head>
    <body>
      <h1>Message Received</h1>
      <p><strong>Email:</strong> ${email}</p>
      <p><strong>Message:</strong> ${message}</p>
      <a href="/">Home</a> | <a href="/submit">Submit Message</a>
    </body>
    </html>
  `);
});

// 404 handler
app.use((req, res) => {
  logger.warn('404 - Page not found', { url: req.url, ip: req.ip });
  res.status(404).send(`
    <!DOCTYPE html>
    <html>
    <head><title>Not Found</title><link rel="stylesheet" href="/styles.css"></head>
    <body>
      <h1>Page Not Found</h1>
      <a href="/">Home</a>
    </body>
    </html>
  `);
});

// Error handler
app.use((err, req, res, next) => {
  logger.error('Server error', { error: err.message, ip: req.ip });
  res.status(500).send(`
    <!DOCTYPE html>
    <html>
    <head><title>Error</title><link rel="stylesheet" href="/styles.css"></head>
    <body>
      <h1>Server Error</h1>
      <p>Something went wrong. Please try again later.</p>
      <a href="/">Home</a>
    </body>
    </html>
  `);
});

app.listen(PORT, () => {
  logger.info(`Server running on port ${PORT}`);
});
