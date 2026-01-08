import React from 'react';
import { Link } from 'react-router-dom';

const Home: React.FC = () => {
  return (
    <div className="page">
      <h1>Hello. Welcome to the Home Page.</h1>
      <Link to="/submit" className="button">Submit Message</Link>
    </div>
  );
};

export default Home;
