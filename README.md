# rubber-pelican

The goal of this device is to be functionally equivalent to the USB rubberDucky by Hak5. Unlike the rubber ducky, the payload is interpreted by a built in duckyscript interpreter and doesn't require compilation. Furthermore, payloads can be run from a web interface. To run this project, open it up with VSCode with the PlatformIO extension, and upload to an ESP32S2 device (native USB required). 

<img width="735" alt="Screenshot 2023-03-04 at 3 02 00 PM" src="https://user-images.githubusercontent.com/1414728/222932540-ca8845e5-e3f0-4dc3-a822-0949c55345ac.png">

## Roadmap
* Evaluate functions when within a command parameter
* Implement random numbers
* Evaluate conditions when && or || is used


## Dependencies

* [tinyexpr](https://github.com/codeplea/tinyexpr)
* [ESP32TinyUSB](https://github.com/chegewara/EspTinyUSB)
* [FastLED](https://fastled.io)
* [React.js](https://reactjs.org)
* [CSS](https://developer.mozilla.org/en-US/docs/Web/CSS)
* [Typescript](https://www.typescriptlang.org)

