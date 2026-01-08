import React, { useState } from 'react';
import './App.css';

function App() {
  const [currentPage, setCurrentPage] = useState('home');
  const [formData, setFormData] = useState({ email: '', message: '' });
  const [result, setResult] = useState(null);

  const handleSubmit = async (e) => {
    e.preventDefault();
    try {
      const response = await fetch('/api/submit', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(formData)
      });
      const data = await response.json();
      setResult(data);
      setCurrentPage('result');
    } catch (error) {
      console.error('Error:', error);
    }
  };

  const HomePage = () => (
    <div className="container">
      <h1>Hello. Welcome to the Home Page.</h1>
      <button onClick={() => setCurrentPage('submit')}>Submit Message</button>
    </div>
  );

  const SubmissionPage = () => (
    <div className="container">
      <h1>Submit Message</h1>
      <form onSubmit={handleSubmit}>
        <div className="form-group">
          <label>Email:</label>
          <input
            type="email"
            value={formData.email}
            onChange={(e) => setFormData({...formData, email: e.target.value})}
            required
          />
        </div>
        <div className="form-group">
          <label>Message:</label>
          <textarea
            value={formData.message}
            onChange={(e) => setFormData({...formData, message: e.target.value})}
            required
          />
        </div>
        <button type="submit">Submit</button>
      </form>
      <a href="#" className="link" onClick={() => setCurrentPage('home')}>Back to Home</a>
    </div>
  );

  const ResultPage = () => (
    <div className="container">
      <h1>Submission Result</h1>
      <div className="result">
        <p><strong>Email:</strong> {result?.email}</p>
        <p><strong>Message:</strong> {result?.message}</p>
      </div>
      <a href="#" className="link" onClick={() => setCurrentPage('submit')}>Back to Submission</a>
      <br />
      <a href="#" className="link" onClick={() => setCurrentPage('home')}>Back to Home</a>
    </div>
  );

  return (
    <div>
      {currentPage === 'home' && <HomePage />}
      {currentPage === 'submit' && <SubmissionPage />}
      {currentPage === 'result' && <ResultPage />}
    </div>
  );
}

export default App;
