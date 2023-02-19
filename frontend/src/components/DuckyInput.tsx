
import { useState } from 'react';
import "./DuckyInput.css";



function DuckyInput() {
    const [textBox, setTextBox] = useState("");
    const [json, setJson] = useState<any>([]);


    function codeBlockToJson(lines: string[]) {
        let newBlock = false;
        let block = [];
        let _json = [];
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
            if (newBlock) {
                block.push(line);
            } else {
                _json.push(lineObject);
            }
            if ("WHILE" in lineObject) {
                newBlock = true;
            }
            if ("ENDWHILE" in lineObject) {
                // block.pop();
                newBlock = false;
                const value = codeBlockToJson(block);
                block = [];
                Object.defineProperty(_json[_json.length -1], "block", { value , enumerable: true}); 
            }
            
        }

        return _json;
    }

    function handleSubmit() {
        const lines = textBox.trim().split("\n");
        const _json = (codeBlockToJson(lines));

        setJson(_json);
        if (!process.env.NODE_ENV || process.env.NODE_ENV === 'development') {
            // console.log(JSON.stringify(_json));
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