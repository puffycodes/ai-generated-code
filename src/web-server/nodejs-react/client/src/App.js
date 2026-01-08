import React, { useState } from 'react';
import './App.css';

function App() {
  const [currentPage, setCurrentPage] = useState('home');
  const [formData, setFormData] = useState({ email: '', message: '' });
  const [submittedData, setSubmittedData] = useState(null);

  const handleSubmit = async (e) => {
    e.preventDefault();
    try {
      const response = await fetch('/api/submit', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(formData)
      });
      const data = await response.json();
      setSubmittedData(data);
      setCurrentPage('result');
    } catch (error) {
      console.error('Error:', error);
    }
  };

  const HomePage = () => (
    <div>
      <h1>Hello. Welcome to the Home Page.</h1>
      <button onClick={() => setCurrentPage('submit')}>Submit Message</button>
    </div>
  );

  const SubmissionPage = () => (
    <div>
      <h1>Submit Message</h1>
      <form onSubmit={handleSubmit}>
        <div>
          <label>Email:</label>
          <input
            type="email"
            value={formData.email}
            onChange={(e) => setFormData({...formData, email: e.target.value})}
            required
          />
        </div>
        <div>
          <label>Message:</label>
          <textarea
            value={formData.message}
            onChange={(e) => setFormData({...formData, message: e.target.value})}
            required
          />
        </div>
        <button type="submit">Submit</button>
      </form>
      <button onClick={() => setCurrentPage('home')}>Back to Home</button>
    </div>
  );

  const ResultPage = () => (
    <div>
      <h1>Submission Result</h1>
      <p><strong>Email:</strong> {submittedData?.email}</p>
      <p><strong>Message:</strong> {submittedData?.message}</p>
      <button onClick={() => setCurrentPage('submit')}>Back to Submission</button>
      <button onClick={() => setCurrentPage('home')}>Back to Home</button>
    </div>
  );

  return (
    <div className="App">
      {currentPage === 'home' && <HomePage />}
      {currentPage === 'submit' && <SubmissionPage />}
      {currentPage === 'result' && <ResultPage />}
    </div>
  );
}

export default App;
