# singing-fish
<img src="https://raw.githubusercontent.com/RocketZ1/singing-fish/main/FishFront.jpg" width=50% height=50%>
This is a project I made where I took a Big Mouth Billy Bass and put an esp8266 microcontroller inside, and controlled the fish's movements by using a web server to communicate between the server and clients.

## How it works
### The circuit
I removed the Big Mouth Billy Bass's electronics from the backing. I stripped the wires that connect to the motor that drives the fish's mouth and tail/head. I attached the wires to a breadboard. The project uses an esp8826 microcontroller to control the fish. I used pins D0, D1, and D2 to control the mouth and D3, D4, and D5 to control the tail/head. Pin D0 is the enable pin for the mouth motor, and D3 is the enable pin for the tail/head. I am using an L298N motor driver to control the polarity and, therefore, the direction of each motor. Providing power to pin D1 or D2 controls the direction of motor 1 and D4, and D5 for motor 2. The motor driver is receiving power from a 9-volt battery. The motor driver's 5-volt output powers the microcontroller, and the microcontroller and motor driver are connected to the same ground. 
## Full circuit
<img src="https://raw.githubusercontent.com/RocketZ1/singing-fish/main/FishFull.jpg" width=50% height=50%>

## Empty Fish
<img src="https://raw.githubusercontent.com/RocketZ1/singing-fish/main/FishEmpty.jpg" width=50% height=50%>

### The server and client programs
The microcontroller is hosting a web server on my local host network. It is constantly listening for a client to connect and receive position data. The server takes the connected client's URL and parses it. The sub-url consists of 3 characters; the first is a number, the selected motor, the second is a comma, and the third is a selected mode. The motor character is either a 1 or 2 for motor 1 or 2, and the mode character is 0, 1, or 2. The modes are 0: off, 1: forward, 2: backward. These modes are what determine the direction of the motors. 

The Python script is a client that takes a requested MIDI song file and parses it using the Python library [python-rtmidi](https://pypi.org/project/python-rtmidi/). It loops through every note in the MIDI song, and depending on the chosen high note (for the fnaf.mid song, it is 64), it will treat the high notes as lyrics and send a post request to the webserver to open the mouth motor if it's not open already and close it when the note finishes. If the current note is lower than the high note, it will move the tail sequentially with the low notes being played. 

# Video
https://www.youtube.com/shorts/7-qWbiSqyS8
