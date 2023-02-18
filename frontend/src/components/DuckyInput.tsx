
import { useState } from 'react';
import "./DuckyInput.css";



function DuckyInput() {
    const [textBox, setTextBox] = useState("");
    const [json, setJson] = useState<any>([]);

    const inputStyle = {
        height: "100px",
        width: "1000px"
    }

    function codeBlockToJson(lines: string[]) {
        const _json = [];
        for (let line of lines) {
            line = line.trim();
            const lineObject = {};
            if (line.indexOf(" ") > -1) {
                const instruction = line.slice(0, line.indexOf(" "));
                const value = line.slice(line.indexOf(" ")+1)
                Object.defineProperty(lineObject, instruction, {value, enumerable: true});
            } else {
                const value = 0;
                Object.defineProperty(lineObject, line, {value, enumerable: true});
            }
            _json.push(lineObject);
        }
        return _json;
    }

    function handleSubmit() {
        const lines = textBox.trim().split("\n");
        const _json = (codeBlockToJson(lines));

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
            <pre>{JSON.stringify(json, null , 2)}</pre>
        </div>
    )
}

export default DuckyInput