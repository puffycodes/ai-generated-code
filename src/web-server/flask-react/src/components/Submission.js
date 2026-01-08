import React, { useState } from 'react';
import { Link, useNavigate } from 'react-router-dom';

function Submission() {
  const [email, setEmail] = useState('');
  const [message, setMessage] = useState('');
  const navigate = useNavigate();

  const handleSubmit = async (e) => {
    e.preventDefault();
    
    const response = await fetch('/api/submit', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ email, message }),
    });
    
    const data = await response.json();
    navigate('/result', { state: { email: data.email, message: data.message } });
  };

  return (
    <div>
      <h1>Submit Message</h1>
      <form onSubmit={handleSubmit}>
        <div className="form-group">
          <label>Email:</label>
          <input
            type="email"
            value={email}
            onChange={(e) => setEmail(e.target.value)}
            required
          />
        </div>
        <div className="form-group">
          <label>Message:</label>
          <textarea
            value={message}
            onChange={(e) => setMessage(e.target.value)}
            required
          />
        </div>
        <button type="submit">Submit</button>
      </form>
      <Link to="/" className="link">Back to Home</Link>
    </div>
  );
}

export default Submission;
