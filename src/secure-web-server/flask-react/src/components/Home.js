import React from 'react';
import { Link } from 'react-router-dom';

function Home() {
  return (
    <div className="container">
      <h1>Hello. Welcome to the Home Page.</h1>
      <Link to="/submit" className="nav-link">Submit Message</Link>
    </div>
  );
}

export default Home;
