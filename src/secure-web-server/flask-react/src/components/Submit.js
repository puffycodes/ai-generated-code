import React, { useState } from 'react';
import { Link, useNavigate } from 'react-router-dom';

function Submit() {
  const [email, setEmail] = useState('');
  const [message, setMessage] = useState('');
  const [error, setError] = useState('');
  const navigate = useNavigate();

  const handleSubmit = async (e) => {
    e.preventDefault();
    setError('');

    try {
      const response = await fetch('/api/submit', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ email, message }),
      });

      const data = await response.json();

      if (response.ok) {
        navigate('/result', { state: { email: data.email, message: data.message } });
      } else {
        setError(data.error || 'Submission failed');
      }
    } catch (err) {
      setError('Network error. Please try again.');
    }
  };

  return (
    <div className="container">
      <h1>Submit Message</h1>
      <form onSubmit={handleSubmit}>
        <div className="form-group">
          <label htmlFor="email">Email:</label>
          <input
            type="email"
            id="email"
            value={email}
            onChange={(e) => setEmail(e.target.value)}
            required
          />
        </div>
        <div className="form-group">
          <label htmlFor="message">Message:</label>
          <textarea
            id="message"
            value={message}
            onChange={(e) => setMessage(e.target.value)}
            required
            maxLength="1000"
          />
        </div>
        <button type="submit">Submit</button>
        {error && <div className="error">{error}</div>}
      </form>
      <Link to="/" className="nav-link">Home</Link>
    </div>
  );
}

export default Submit;
