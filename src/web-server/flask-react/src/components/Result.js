import React from 'react';
import { Link, useLocation } from 'react-router-dom';

function Result() {
  const location = useLocation();
  const { email, message } = location.state || {};

  return (
    <div>
      <h1>Submission Result</h1>
      <div className="result">
        <p><strong>Email:</strong> {email}</p>
        <p><strong>Message:</strong> {message}</p>
      </div>
      <Link to="/submit" className="link">Back to Submission</Link>
      <br />
      <Link to="/" className="link">Back to Home</Link>
    </div>
  );
}

export default Result;
