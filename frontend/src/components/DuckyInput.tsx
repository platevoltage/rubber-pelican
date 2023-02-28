
import { useState, useEffect, useRef } from 'react';
import "./DuckyInput.css";

const keyWords = [
    "STRING ",
    "STRINGLN ",
    "DELAY "
]

function processText(text: string) {
    let html = text
    for (let word of keyWords) {
        html = html.replace(word, `<span style='color: #ff0000'>${word}</span>`);
    }
    return html;
}

function DuckyInput() {
    const [textBox, setTextBox] = useState("");
    const textDisplay = useRef(null);
    useEffect(() => {
        
        if (textDisplay.current) {
            const div = textDisplay.current as HTMLDivElement;
            const pre = div.childNodes[0] as HTMLDivElement;
            const html = processText(textBox);
            pre.innerHTML = html;

            // pre.innerHTML
            console.log(pre.innerHTML);
            // textDisplay.current.innerHTML = textBox;
        }
        
    },[textBox]);

    function handleSave() {
        if (!process.env.NODE_ENV || process.env.NODE_ENV === 'development') {
            // console.log(JSON.stringify(_json));
            return fetch(`http://test.local/save`, {
                headers: {'Content-Type': 'text/plain'},
                method: 'POST',
                body: textBox
            });
        }
        else {
            return fetch(`/save`, {
                headers: {'Content-Type': 'text/plain'},
                method: 'POST',
                body: textBox
            });
        }
    }

    function handleSubmit() {
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
    async function handleRecover() {
        let response;
        if (!process.env.NODE_ENV || process.env.NODE_ENV === 'development') {
            // console.log(JSON.stringify(_json));
            response = await fetch(`http://test.local/recover`, {
                headers: {'Content-Type': 'text/plain'},
                method: 'GET',
            });
        }
        else {
            response = await fetch(`/recover`, {
                headers: {'Content-Type': 'text/plain'},
                method: 'GET',
            });
        }
        const body = await response.text();
        setTextBox(body);
    }

    return (
        <div className="DuckyInput">
                <div className="overlay" ref={textDisplay}>
                    <pre></pre>
                </div>

       
            <div>
                <button onClick={handleSubmit}>Run</button>
                <button onClick={handleRecover}>Recover</button>
                <button onClick={handleSave}>Save</button>
            </div>
            {/* <pre>{JSON.stringify(json, null , 2)}</pre> */}
            <textarea
                    value={textBox}
                    name="strip-length"
                    onChange={(e) => {setTextBox(e.target.value)}}
                    placeholder=""
                    spellCheck={false}
            />
        </div>
    )
}

export default DuckyInput