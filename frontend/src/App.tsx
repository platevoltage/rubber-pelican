// import React from 'react';
// import logo from './logo.svg';
import './App.css';
import { useState } from 'react';

function App() {
  const [textBox, setTextBox] = useState("");

  function handleSubmit() {
    return fetch(`http://test.local/typestring?string=${textBox}`);
  }

  return (
    <div className="App">
      <header className="App-header">
        {/* <img src={logo} className="App-logo" alt="logo" /> */}
        <input
                value={textBox}
                name="strip-length"
                onChange={(e) => {setTextBox(e.target.value)}}
                type="text"
                placeholder=""
                // style={inputStyle}
        />
        <button onClick={handleSubmit}>Send</button>
     
        <p>
          Edit <code>src/App.tsx</code> and save to reload.
        </p>
        <a
          className="App-link"
          href="https://reactjs.org"
          target="_blank"
          rel="noopener noreferrer"
        >
          Learn React
        </a>
      </header>
    </div>
  );
}

export default App;
