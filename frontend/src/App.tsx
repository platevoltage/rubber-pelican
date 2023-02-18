// import React from 'react';
// import logo from './logo.svg';
import './App.css';
import { useState } from 'react';

function App() {
  const [textBox, setTextBox] = useState("");
  const [json, setJson] = useState<any>([]);

  const inputStyle = {
    height: "100px",
    width: "1000px"
  }

  function handleSubmit() {
    const lines = textBox.trim().split("\n");
    const _json = [];
    for (let line of lines) {
      line = line.trim();
      const instruction = line.slice(0, line.indexOf(" "));
      const value = line.slice(line.indexOf(" ")+1)
      const lineObject = {};
      Object.defineProperty(lineObject, instruction, {value, enumerable: true});
      _json.push(lineObject);

    }
    console.log(JSON.stringify(_json));
    setJson(_json);


    return fetch(`http://test.local/duckyscript?string=${JSON.stringify(_json)}`);
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
          {JSON.stringify(json)}
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
