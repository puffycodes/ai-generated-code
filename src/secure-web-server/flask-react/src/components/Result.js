import React from 'react';
import { Link, useLocation } from 'react-router-dom';

function Result() {
  const location = useLocation();
  const { email, message } = location.state || {};

  if (!email || !message) {
    return (
      <div className="container">
        <h1>No Data</h1>
        <p>No submission data found.</p>
        <Link to="/" className="nav-link">Home</Link>
      </div>
    );
  }

  return (
    <div className="container">
      <h1>Submission Received</h1>
      <div className="result">
        <p><strong>Email:</strong> {email}</p>
        <p><strong>Message:</strong> {message}</p>
      </div>
      <Link to="/submit" className="nav-link">Submit Another Message</Link>
      <Link to="/" className="nav-link">Home</Link>
    </div>
  );
}

export default Result;
