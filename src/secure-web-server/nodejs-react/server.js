const express = require('express');
const helmet = require('helmet');
const rateLimit = require('express-rate-limit');
const cors = require('cors');
const session = require('express-session');
const { body, validationResult } = require('express-validator');
const path = require('path');
const logger = require('./utils/logger');
const db = require('./models/database');

const app = express();
const PORT = process.env.PORT || 3001;

// Security middleware
app.use(helmet({
  contentSecurityPolicy: {
    directives: {
      defaultSrc: ["'self'"],
      styleSrc: ["'self'", "'unsafe-inline'"],
      scriptSrc: ["'self'"],
      imgSrc: ["'self'", "data:", "https:"]
    }
  }
}));

// Rate limiting
const limiter = rateLimit({
  windowMs: 15 * 60 * 1000, // 15 minutes
  max: 100, // limit each IP to 100 requests per windowMs
  message: 'Too many requests from this IP'
});
app.use(limiter);

// CORS
app.use(cors({
  origin: process.env.NODE_ENV === 'production' ? false : 'http://localhost:3000',
  credentials: true
}));

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

app.use(express.json({ limit: '10mb' }));
app.use(express.urlencoded({ extended: true, limit: '10mb' }));

// Serve static files from React build
if (process.env.NODE_ENV === 'production') {
  app.use(express.static(path.join(__dirname, 'client/build')));
}

// Input validation middleware
const validateMessage = [
  body('email').isEmail().normalizeEmail().escape(),
  body('message').trim().isLength({ min: 1, max: 1000 }).escape()
];

// API Routes
app.post('/api/submit', validateMessage, async (req, res) => {
  try {
    const errors = validationResult(req);
    if (!errors.isEmpty()) {
      logger.warn('Validation failed', { errors: errors.array(), ip: req.ip });
      return res.status(400).json({ errors: errors.array() });
    }

    const { email, message } = req.body;
    
    // Store in database (prevents SQL injection with parameterized queries)
    const submission = await db.Message.create({
      email,
      message,
      submittedAt: new Date()
    });

    logger.info('Message submitted', { id: submission.id, email, ip: req.ip });
    
    res.json({
      success: true,
      data: {
        id: submission.id,
        email: submission.email,
        message: submission.message
      }
    });
  } catch (error) {
    logger.error('Submission error', { error: error.message, ip: req.ip });
    res.status(500).json({ error: 'Internal server error' });
  }
});

// Catch all handler for React routes
if (process.env.NODE_ENV === 'production') {
  app.get('*', (req, res) => {
    res.sendFile(path.join(__dirname, 'client/build', 'index.html'));
  });
}

// Error handling middleware
app.use((error, req, res, next) => {
  logger.error('Unhandled error', { error: error.message, stack: error.stack });
  res.status(500).json({ error: 'Internal server error' });
});

// Initialize database and start server
db.sequelize.sync().then(() => {
  app.listen(PORT, () => {
    logger.info(`Server running on port ${PORT}`);
  });
}).catch(err => {
  logger.error('Database connection failed', { error: err.message });
});
