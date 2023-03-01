# Hackathon 2

## Tip 1
All the code below has been designed to be easily split up and ```Copy-Pasted``` into the sketches below. Think of the code as ```Lego Bricks``` that you can use to build your own solution.

## Tip 2
Right click the links and press "Open in new tab"


## Links to the ```Copy-Paste``` example codes
- Utility Code
  - [GetMacAddress.ino](./ExampleCode/GetMacAddress/GetMacAddress.ino)
    - Will print the mac address of the ESP32 to the Serial Monitor
- Demo Code
  - [PrintJsonDemo.ino](./ExampleCode/PrintJsonDemo/PrintJsonDemo.ino)
    - Will print a json object to the Serial Monitor. Run this code to see what the json strings sent between the ESPs look like.
  - [ReadJsonDemo.ino](./ExampleCode/ReadJsonDemo/ReadJsonDemo.ino)
    - Will read a json object from the Serial Monitor. Paste the code from [PrintJsonDemo.ino](./ExampleCode/PrintJsonDemo/PrintJsonDemo.ino) into the Serial Monitor, and the code will extract the information and present it in a more readable format. Run this code to see how json strings are read and used by the ArduinoJson library.
- Split and ```Copy-Paste``` Code
  - [FanController.ino](./ExampleCode/FanController/FanController.ino)
    - Contains a solution to Hackathon 1 designed to be easily split up and ```Copy-Pasted``` into the sketches below.
- ```Copy-Paste``` Code
  - [Remote.ino](./ExampleCode/Remote/Remote.ino)
    - Contains everything necessary for the remote to communicate using Json over ESP_NOW.
  - [Router.ino](./ExampleCode/Router/Router.ino)
    - Contains everything necessary for the router to communicate using Json over ESP_NOW and Serial1.
  - [Fan.ino](./ExampleCode/Fan/Fan.ino)
    - Contains everything necessary for the fan to communicate using Json over ESP_NOW.
  - [WebServerSerial.ino](./ExampleCode/WebServerSerial/WebServerSerial.ino)
    - Contains everything necessary for the WebServer ESP to communicate with the Router using Json over Serial1.
  - [WebServerWebsite.ino](./ExampleCode/WebServerWebsite/WebServerWebsite.ino)
    - Contains everything necessary for the WebServer ESP to create a WiFi access point and serve a website.
    - After connecting your Phone/PC to the ESP32's WiFi access point, the website will be available at [http://192.168.1.1:80](http://192.168.1.1:80)

# Potential bugs and possible causes/solutions

- Not compiling
  - Check that the ArduinoJson library is installed
  - Check that the ESP32 Dev board is selected
- Mac adress is incorrect
  - The mac address is unchanged and the esp is instructing to change it
    - Upload sketch on other esp and enter that address into the code for the esp that is not working
- The mac address has the wrong syntax
  - The compiler is reporting syntax errors, perhaps containing the character '{'
    - Chek that every hex number starts with 0x and has a comma at the end
    - Check that the last hex number has no comma at the end
    - Check that the closing '}' at the end of the mac adress is followed by a comma
    - Check that the closing '}' at the end of the mac adress is not followed by a semicolon
- In the Serial Monitor: "`Stack canary watchpoint triggered (loopTask)`"
  - You created a function with the return type DynamicJsonDocument but forgot to return a value
- Random symbols come out of the Serial Monitor
  - Check that the Serial Monitors baudrate is configured to the same as `Serial.begin(baudRate)` in `void setup()`
- Serial Monitor: "`deserializeJson() failed with the error message: NoMemory`"
  - The received message is larger than what can be stored in the json document.
  - Increase the number 500 inside `DynamicJsonDocument doc(500);` to a higher number.
- `initial value of reference to non-const must be an lvalue`
  - You are trying to use the return value of a function as an argument to another function, without first making a variable.
  - Create a variable for the return value of the first function, then pass that variable as an argument to the second function instead.
