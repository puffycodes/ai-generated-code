import React from 'react';
import { Link, useLocation } from 'react-router-dom';

function Result() {
  const location = useLocation();
  const data = location.state;

  if (!data) {
    return (
      <div className="container">
        <h1>No Data Found</h1>
        <Link to="/" className="nav-link">Back to Home</Link>
      </div>
    );
  }

  return (
    <div className="container">
      <h1>Submission Received</h1>
      <div className="result-box">
        <h3>Your Submission:</h3>
        <p><strong>Email:</strong> {data.email}</p>
        <p><strong>Message:</strong> {data.message}</p>
      </div>
      <Link to="/submit" className="nav-link">Submit Another Message</Link>
      <Link to="/" className="nav-link">Back to Home</Link>
    </div>
  );
}

export default Result;
