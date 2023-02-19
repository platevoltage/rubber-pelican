
import { useState } from 'react';
import "./DuckyInput.css";



function DuckyInput() {
    const [textBox, setTextBox] = useState("");
    const [json, setJson] = useState<any>([]);


    function codeBlockToJson(lines: string[]) {
        let newBlock = false;
        let block = []; //holds lines that come after a while loop
        let _json = []; //will be returned at the end
        for (let line of lines) {
            line = line.trim();
            const lineObject = {};
            if (line.indexOf(" ") > -1) {
                const instruction = line.slice(0, line.indexOf(" ")); //get instruction
                const value = line.slice(line.indexOf(" ")+1); //everything after instruction
                Object.defineProperty(lineObject, instruction, {value, enumerable: true}); //creates key/value pair from 2 vars
            } else {
                const value = 0;
                Object.defineProperty(lineObject, line, {value, enumerable: true}); //if no value, makes value 0
            }
            if (newBlock) {
                block.push(line); //if we are inside a block, it puts lines in block array
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
                body: textBox
            });
        }
        else {
            return fetch(`/duckyscript`, {
                headers: {'Content-Type': 'text/plain'},
                method: 'POST',
                body: textBox
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
            {/* <pre>{JSON.stringify(json, null , 2)}</pre> */}
        </div>
    )
}

export default DuckyInput