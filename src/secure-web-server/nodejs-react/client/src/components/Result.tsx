import React from 'react';
import { Link, useLocation } from 'react-router-dom';

interface ResultData {
  id: number;
  email: string;
  message: string;
}

const Result: React.FC = () => {
  const location = useLocation();
  const data = location.state?.data as ResultData;

  if (!data) {
    return (
      <div className="page">
        <h1>No Data Available</h1>
        <Link to="/" className="link">Back to Home</Link>
      </div>
    );
  }

  return (
    <div className="page">
      <h1>Message Submitted Successfully</h1>
      
      <div className="result-display">
        <p><strong>Email:</strong> {data.email}</p>
        <p><strong>Message:</strong> {data.message}</p>
      </div>
      
      <div className="links">
        <Link to="/submit" className="button">Submit Another Message</Link>
        <Link to="/" className="link">Back to Home</Link>
      </div>
    </div>
  );
};

export default Result;
