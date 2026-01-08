import React, { useState } from 'react';
import { Link, useNavigate } from 'react-router-dom';
import DOMPurify from 'dompurify';

const Submission: React.FC = () => {
  const [email, setEmail] = useState('');
  const [message, setMessage] = useState('');
  const [errors, setErrors] = useState<string[]>([]);
  const [loading, setLoading] = useState(false);
  const navigate = useNavigate();

  const validateInput = (): boolean => {
    const newErrors: string[] = [];
    
    if (!email || !/^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(email)) {
      newErrors.push('Valid email is required');
    }
    
    if (!message || message.trim().length === 0) {
      newErrors.push('Message is required');
    }
    
    if (message.length > 1000) {
      newErrors.push('Message must be less than 1000 characters');
    }
    
    setErrors(newErrors);
    return newErrors.length === 0;
  };

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    
    if (!validateInput()) return;
    
    setLoading(true);
    
    try {
      const sanitizedData = {
        email: DOMPurify.sanitize(email),
        message: DOMPurify.sanitize(message)
      };
      
      const response = await fetch('/api/submit', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(sanitizedData),
      });
      
      const data = await response.json();
      
      if (response.ok) {
        navigate('/result', { state: { data: data.data } });
      } else {
        setErrors(data.errors?.map((err: any) => err.msg) || ['Submission failed']);
      }
    } catch (error) {
      setErrors(['Network error occurred']);
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="page">
      <h1>Submit Message</h1>
      
      {errors.length > 0 && (
        <div className="errors">
          {errors.map((error, index) => (
            <p key={index} className="error">{error}</p>
          ))}
        </div>
      )}
      
      <form onSubmit={handleSubmit}>
        <div className="form-group">
          <label htmlFor="email">Email:</label>
          <input
            type="email"
            id="email"
            value={email}
            onChange={(e) => setEmail(e.target.value)}
            required
            maxLength={254}
          />
        </div>
        
        <div className="form-group">
          <label htmlFor="message">Message:</label>
          <textarea
            id="message"
            value={message}
            onChange={(e) => setMessage(e.target.value)}
            required
            maxLength={1000}
            rows={5}
          />
        </div>
        
        <button type="submit" disabled={loading} className="button">
          {loading ? 'Submitting...' : 'Submit'}
        </button>
      </form>
      
      <Link to="/" className="link">Back to Home</Link>
    </div>
  );
};

export default Submission;
