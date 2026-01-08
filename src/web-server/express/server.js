const express = require('express');
const path = require('path');
const app = express();
const PORT = 3000;

app.use(express.urlencoded({ extended: true }));
app.use(express.static('public'));

app.get('/', (req, res) => {
  res.send(`
    <!DOCTYPE html>
    <html>
    <head>
      <title>Home Page</title>
      <link rel="stylesheet" href="/styles.css">
    </head>
    <body>
      <h1>Hello. Welcome to the Home Page.</h1>
      <a href="/submit">Submit Message</a>
    </body>
    </html>
  `);
});

app.get('/submit', (req, res) => {
  res.send(`
    <!DOCTYPE html>
    <html>
    <head>
      <title>Submit Message</title>
      <link rel="stylesheet" href="/styles.css">
    </head>
    <body>
      <h1>Submit Message</h1>
      <form method="POST" action="/submit">
        <label for="email">Email:</label>
        <input type="email" id="email" name="email" required><br><br>
        <label for="message">Message:</label>
        <textarea id="message" name="message" required></textarea><br><br>
        <button type="submit">Submit</button>
      </form>
      <a href="/">Back to Home</a>
    </body>
    </html>
  `);
});

app.post('/submit', (req, res) => {
  const { email, message } = req.body;
  res.send(`
    <!DOCTYPE html>
    <html>
    <head>
      <title>Message Received</title>
      <link rel="stylesheet" href="/styles.css">
    </head>
    <body>
      <h1>Message Received</h1>
      <p><strong>Email:</strong> ${email}</p>
      <p><strong>Message:</strong> ${message}</p>
      <a href="/submit">Submit Another Message</a><br>
      <a href="/">Back to Home</a>
    </body>
    </html>
  `);
});

app.listen(PORT, () => {
  console.log(`Server running on http://localhost:${PORT}`);
});
