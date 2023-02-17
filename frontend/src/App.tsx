// import React from 'react';
// import logo from './logo.svg';
import './App.css';
import { useState } from 'react';

function App() {
  const [textBox, setTextBox] = useState("");

  const inputStyle = {
    height: "100px",
    width: "1000px"
  }

  function handleSubmit() {
    return fetch(`http://test.local/duckyscript?string=${textBox}`);
  }

  return (
    <div className="App">
      <header className="App-header">
        {/* <img src={logo} className="App-logo" alt="logo" /> */}
        <textarea
                value={textBox}
                name="strip-length"
                onChange={(e) => {setTextBox(e.target.value)}}
                // type="text"
                placeholder=""
                // height="40"
                style={inputStyle}
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
