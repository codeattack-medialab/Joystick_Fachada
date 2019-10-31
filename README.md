# Joystick_Fachada
## Introduction

This is a remote, open source wireless-based interfacing project we built up for the [Medialab-Prado digital facade](https://www.medialab-prado.es/noticias/fachada-digital-informacion-tecnica-0), designed for the **MadPong Game by #codeAttack**. 

- Through a *NodeMCU controller with a Joystick interface* we send UDP-based data against the UDP Server build in the *Godot Game* to react accordingly. 
- Check "nodejs" branch to run a Nodejs solution. Nodemcu will connect with [websockets](https://socket.io/docs/) server build with python then forward UDP-based data to a *Godot Game*. This solution might help to allow a Joystick to Joysick communication or for a easy server to clients comunitacion protocol. 
- For further reference, *we will add soon the Madpong Game source code here (TBD)*.

Some screenshots and videos of the final project are [available in this gallery](https://www.flickr.com/photos/carlesgutierrez/sets/72157711220061241/).

## Software Requirements

- [NodeMCU drivers](https://github.com/nodemcu/nodemcu-devkit/tree/master/Drivers), 
- [Arduino IDE](https://www.arduino.cc/en/Main/Software), [NodeMCU Interface libraries](http://arduino.esp8266.com/stable/package_esp8266com_index.json)

- You will need [Git](https://git-scm.com/) to clone and update this repository
- We recommend a [Linux-based Environment](https://www.debian.org/),
- Alternatively a [Brew-based environment](https://brew.sh/) for Mac OS, 
- Finally you could use a [WSL-based environment](https://docs.microsoft.com/en-us/windows/wsl/install-win10) over your latest Wintel OS.
- Install Arduino Librarie: ArduinoJson

## Hardware Requirements. Bill of Materials (BOM, TBD)

- 1x [NodeMCU controller](https://www.nodemcu.com/index_en.html#fr_54747661d775ef1a3600009e), we recommend the latest version.
- 1x [Analog Joystick](https://exploreembedded.com/wiki/Analog_JoyStick_with_Arduino)
- 1x [Small Breadboard](https://en.wikipedia.org/wiki/Breadboard)
- 1x [Micro-B USB Data cable](https://en.wikipedia.org/wiki/USB_hardware#Micro_connectors)
- 1x [e-InkPaper 2.9" BW](https://tienda.bricogeek.com/pantallas-e-paper-tinta-electronica/1021-pantalla-e-paper-spi-29-296x128-monocromo.html?search_query=e-paper++2.9&results=6)
- 1x 4.7k Resistor for this [NodeMcu Issue](https://github.com/esp8266/Arduino/issues/2466). Solution is set a 4.7k resistor between GPIO15 (D8) and GND.


## Getting Started

Firstly, clone this repository in your environment. https://github.com/codeattack-medialab/Joystick_Fachada

```git clone https://github.com/codeattack-medialab/Joystick_Fachada```

- Don't forget to [check if you have the appropriate NodeMCU drivers](https://github.com/nodemcu/nodemcu-devkit/tree/master/Drivers) for your OS
- Follow the [tutorial to connect/prepare your NodeMCU on Arduino IDE](https://www.instructables.com/id/Quick-Start-to-Nodemcu-ESP8266-on-Arduino-IDE/)
- e-InkPaper 2.9" BW has his own pin mapping. Once 4.7k Resistor solution has been done. Follow this pin mapping to connect your e-InkPaper to the NodeMcu: BUSY -> D2,    RST -> D4,    DC -> D3,    CS -> D8,     CLK -> D5,     DIN -> D7,     GND -> GND, 3.3V -> 3.3V 
- testers: Go to arduino/testers folder if you want to try all diferents functionallities for a better undestanding. 

## E-Paper Screen Images

Use following online software (https://javl.github.io/image2cpp/) to convert and save images in the e-paper screen format. Each pixel is represented inside a Byte in Hexa format. 
Instrucctions: 
 - Set your BW or gray image. Chek resolutio or try to fit the image to our screen resolution: 296x128
 - Set you desired threslhod value. And check the preview results. 
 - Set the resulting "code output format" to ARDUINO and copy/paste the resulting code inside our code: ( IMG_0001.ino )

## UDP Protocol

NodeMCU sends a String with :("idJoystick"/"X or Click"/"Float") to the *godot game* IP and Port "33333"
e.g: 
- 1/X/1.0      (Joystick 1 sends maximum Right value)
- 1/X/0.0      (Joystick 1 sends "no movement" value)
- 1/X/-1.0     (Joystick 1 sends maximum Left value)
- 2/Click/1.0  (Joystick 2 sends Click event Pressed value)

## Contributors
[CodeAttack](https://www.medialab-prado.es/actividades/code-attack)

[Carles Gutiérrez](http://carlesgutierrez.github.io/)

[Fablab Medialab-Prado](https://www.medialab-prado.es/programas/fablab-laboratorio-de-fabricacion-digital)
