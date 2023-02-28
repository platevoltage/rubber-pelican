
import { useState, useEffect, useRef } from 'react';
import "./DuckyInput.css";

const commands = [
    "STRING",
    "STRINGLN",
    "DELAY",
    "REM",
    "DEFINE",
    "LED_R",
    "LED_G",
    "LED_B",
    "LED_OFF",
    "VAR",
    "WHILE",
    "IF",
    "ELSE",
    "THEN",
    "END_WHILE",
    "END_IF",
    "FUNCTION",
    "END_FUNCTION",
    "RETURN",
    "HOLD",
    "RELEASE",
    "INJECT_MOD",
    "WAIT_FOR_BUTTON_PRESS",
    "WAIT_FOR_CAPS_ON",
    "WAIT_FOR_CAPS_OFF",
    "WAIT_FOR_CAPS_CHANGE",
    "WAIT_FOR_NUM_ON",
    "WAIT_FOR_NUM_OFF",
    "WAIT_FOR_NUM_CHANGE",
    "WAIT_FOR_SCROLL_ON",
    "WAIT_FOR_SCROLL_OFF",
    "WAIT_FOR_SCROLL_CHANGE",
    "ATTACKMODE"
]
const params = [
    "MAN_",
    "PROD_",
    "SERIAL_",
    "VID_",
    "PID_",
    "HID",
    "STORAGE"
]
const otherKeyWords = [
    "TRUE",
    "FALSE",
]

function highlightText(text: string) {
    let html = text
    const commandColor = "#aaaaff";
    for (let word of commands) {
        html = html.replace(`${word} `, `<span style='color: ${commandColor}'>${word}</span> `);
        html = html.replace(`${word}\t`, `<span style='color: ${commandColor}'>${word}</span>\t`);
        html = html.replace(`${word}\n`, `<span style='color: ${commandColor}'>${word}</span>\n`);
        if (text.endsWith(word)) {

            html = html.replace(`${word}`, `<span style='color: ${commandColor}'>${word}</span>`);
        }
    }
    const keywordColor = "#ffffaa";
    for (let word of otherKeyWords) {
        html = html.replace(`${word} `, `<span style='color: ${keywordColor}'>${word}</span> `);
        html = html.replace(`${word}\t`, `<span style='color: ${keywordColor}'>${word}</span>\t`);
        html = html.replace(`${word}\n`, `<span style='color: ${keywordColor}'>${word}</span>\n`);
        if (text.endsWith(word)) {

            html = html.replace(`${word}`, `<span style='color: ${keywordColor}'>${word}</span>`);
        }
    }
    const paramsColor = "#aaffaa";
    for (let word of params) {
        html = html.replace(`${word} `, `<span style='color: ${paramsColor}'>${word}</span> `);
        html = html.replace(`${word}\t`, `<span style='color: ${paramsColor}'>${word}</span>\t`);
        html = html.replace(`${word}\n`, `<span style='color: ${paramsColor}'>${word}</span>\n`);
        if (text.endsWith(word) || word.endsWith("_")) {

            html = html.replace(`${word}`, `<span style='color: ${paramsColor}'>${word}</span>`);
        }
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
            const html = highlightText(textBox);
            pre.innerHTML = html;


            // pre.innerHTML
            console.log(pre.innerHTML);
            // textDisplay.current.innerHTML = textBox;
        }
        function handleTextChange(e: KeyboardEvent) {
            if (e.key === 'Tab') {
                e.preventDefault();
                setTextBox(textBox + "\t");
            }
        }
        window.addEventListener("keydown", handleTextChange);
        return  () => {
            window.removeEventListener("keydown", handleTextChange);
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