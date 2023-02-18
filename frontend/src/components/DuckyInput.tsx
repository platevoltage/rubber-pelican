
import { useState } from 'react';
import "./DuckyInput.css";



function DuckyInput() {
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
        setJson(_json);
        if (!process.env.NODE_ENV || process.env.NODE_ENV === 'development') {
            console.log(JSON.stringify(_json));
            return fetch(`http://test.local/duckyscript`, {
                headers: {'Content-Type': 'text/plain'},
                method: 'POST',
                body: JSON.stringify(_json)
            });
        }
        else {
            return fetch(`/duckyscript`, {
                headers: {'Content-Type': 'text/plain'},
                method: 'POST',
                body: JSON.stringify(_json)
            });
        }
    }

    return (
        <div className="DuckyInput">
            <textarea
                value={textBox}
                name="strip-length"
                onChange={(e) => {setTextBox(e.target.value)}}
                // type="text"
                placeholder=""
                spellCheck={false}
                // height="40"
            />
            <button onClick={handleSubmit}>Send</button>
        </div>
    )
}

export default DuckyInput