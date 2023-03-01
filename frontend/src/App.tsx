import { useState } from 'react';
// import logo from './logo.svg';
import './App.css';
import DuckyInput from './components/DuckyInput';
import ButtonBlock from './components/ButtonBlock';


function App() {
  const [textBox, setTextBox] = useState("");

  return (
    <div className="App">
      <DuckyInput textBox={textBox} setTextBox={setTextBox} />
      <ButtonBlock textBox={textBox} setTextBox={setTextBox} />
    </div>
  );
}

export default App;
