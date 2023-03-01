import React from 'react'


interface Props {
    textBox: string;
    setTextBox: (text: string) => void;
}

export default function ButtonBlock({textBox, setTextBox}: Props) {
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
        <div>
            <button onClick={handleSubmit}>Run</button>
            <button onClick={handleRecover}>Recover</button>
            <button onClick={handleSave}>Save</button>
        </div>
  )
}
