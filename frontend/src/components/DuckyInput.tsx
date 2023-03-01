
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
    "END_WHILE",
    "WHILE",
    "END_IF",
    "IF",
    "ELSE",
    "THEN",
    "END_FUNCTION",
    "FUNCTION",
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
    "()",
    
]
const nonPrintingKeys = [
    "COMMAND",
    "WINDOWS",
    "GUI",
    "CTRL",
    "OPTION",
    "ALT",
    "SHIFT",
    "ESCAPE",
    "ENTER",
    "SPACE",
    "TAB",
    "UPARROW",
    "DOWNARROW",
    "LEFTARROW",
    "RIGHTARROW",
    "UP",
    "DOWN",
    "LEFT",
    "RIGHT",
    "PAGEUP",
    "PAGEDOWN",
    "HOME",
    "END",
    "INSERT",
    "DELETE",
    "BACKSPACE",
    "PAUSE",
    "BREAK",
    "PRINTSCREEN",
    "MENU",
    "APP",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "CAPSLOCK",
    "NUMLOCK",
    "SCROLLLOCK"

]

function highlightText(text: string) {
    let html = text
    const commandColor = "#aaaaff";
    for (let word of commands) {
        html = html.replaceAll(`${word} `, `<span style='color: ${commandColor}'>${word}</span> `);
        html = html.replaceAll(`${word}\t`, `<span style='color: ${commandColor}'>${word}</span>\t`);
        html = html.replaceAll(`${word}\n`, `<span style='color: ${commandColor}'>${word}</span>\n`);
        if (text.endsWith(word)) {

            html = html.replaceAll(`${word}`, `<span style='color: ${commandColor}'>${word}</span>`);
        }
    }
    const nonPrintingColor = "#ffaaff";
    for (let word of nonPrintingKeys) {
        html = html.replaceAll(`${word} `, `<span style='color: ${nonPrintingColor}'>${word}</span> `);
        html = html.replaceAll(`${word}\t`, `<span style='color: ${nonPrintingColor}'>${word}</span>\t`);
        html = html.replaceAll(`${word}\n`, `<span style='color: ${nonPrintingColor}'>${word}</span>\n`);
        html = html.replaceAll(`${word}-`, `<span style='color: ${nonPrintingColor}'>${word}</span>-`);
        if (text.endsWith(word)) {

            html = html.replaceAll(`${word}`, `<span style='color: ${nonPrintingColor}'>${word}</span>`);
        }
    }
    const keywordColor = "#ffffaa";
    for (let word of otherKeyWords) {
        html = html.replaceAll(`${word} `, `<span style='color: ${keywordColor}'>${word}</span> `);
        html = html.replaceAll(`${word}\t`, `<span style='color: ${keywordColor}'>${word}</span>\t`);
        html = html.replaceAll(`${word}\n`, `<span style='color: ${keywordColor}'>${word}</span>\n`);
        if (text.endsWith(word)) {

            html = html.replaceAll(`${word}`, `<span style='color: ${keywordColor}'>${word}</span>`);
        }
    }
    const paramsColor = "#aaffaa";
    for (let word of params) {
        html = html.replaceAll(`${word} `, `<span style='color: ${paramsColor}'>${word}</span> `);
        html = html.replaceAll(`${word}\t`, `<span style='color: ${paramsColor}'>${word}</span>\t`);
        html = html.replaceAll(`${word}\n`, `<span style='color: ${paramsColor}'>${word}</span>\n`);
        if (text.endsWith(word) || word.endsWith("_")) {

            html = html.replaceAll(`${word}`, `<span style='color: ${paramsColor}'>${word}</span>`);
        }
    }

    return html + "\n";
}

interface Props {
    textBox: string;
    setTextBox: (text: string) => void;
}

function DuckyInput({textBox, setTextBox}: Props) {

    const textDisplay = useRef(null);
    const hiddenText = useRef(null);
    
    function handleScroll() {
        if (hiddenText.current) {
            const hiddenDiv = hiddenText.current as HTMLDivElement;
            const hiddenTextBox = hiddenDiv.childNodes[0] as HTMLDivElement;
            const scrollPosition = hiddenTextBox.scrollTop;
            if (textDisplay.current) {
                const div = textDisplay.current as HTMLDivElement;
                div.scrollTop = scrollPosition;
            }
        }
    };
    useEffect(() => {
        let hiddenTextObject = hiddenText.current;
        let hiddenTextBox: HTMLTextAreaElement;
        let cursor: number;
        if (textDisplay.current) {
            const div = textDisplay.current as HTMLDivElement;
            const pre = div.childNodes[0] as HTMLDivElement;
            const html = highlightText(textBox);
            pre.innerHTML = html;
        }
        let key = 0;
        function handleTextChange(e: KeyboardEvent) {
            if (hiddenTextObject) {
                const hiddenDiv = hiddenTextObject as HTMLDivElement;
                hiddenTextBox = hiddenDiv.childNodes[0] as HTMLTextAreaElement;
                cursor = hiddenTextBox.selectionEnd;
            }
            if (e.key === 'Tab') {
                e.preventDefault();
                key = 4;
                setTextBox(`${textBox.substring(0, cursor)}    ${textBox.substring(cursor, textBox.length)}`);
            }
            if (e.key === 'Backspace') {
                e.preventDefault();
                key = -1;
                setTextBox(`${textBox.substring(0, cursor-1)}${textBox.substring(cursor, textBox.length)}`);
            }
        }
        window.addEventListener("keydown", handleTextChange);
        return  () => {
            window.removeEventListener("keydown", handleTextChange);
            if (hiddenTextObject) {
                if (key !== 0) hiddenTextBox?.setSelectionRange(cursor+key,cursor+key, "none");
            }
        }
    },[textBox]);

    return (
        <div className="DuckyInput">
            <div className="overlay" ref={textDisplay}>
                <pre></pre>
            </div>

            <div className="hiddenText" ref={hiddenText}>
                <textarea
                    value={textBox}
                    name="strip-length"
                    onScroll={handleScroll}
                    onChange={(e) => {setTextBox(e.target.value)}}
                    placeholder=""
                    spellCheck={false}
                />
            </div>
        </div>
    )
}

export default DuckyInput