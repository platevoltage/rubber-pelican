
import { useState } from 'react';
import "./DuckyInput.css";



function DuckyInput() {
    const [textBox, setTextBox] = useState("");
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
            <textarea
                value={textBox}
                name="strip-length"
                onChange={(e) => {setTextBox(e.target.value)}}
                // type="text"
                placeholder=""
                spellCheck={false}
                // height="40"
            />
            <div>
                <button onClick={handleSubmit}>Run</button>
                <button onClick={handleRecover}>Recover</button>
                <button onClick={handleSave}>Save</button>
            </div>
            {/* <pre>{JSON.stringify(json, null , 2)}</pre> */}
        </div>
    )
}

export default DuckyInput