#include <WiFi.h>
#include <WebServer.h>

int motorSpeed = 0;

// Below is the SSID and password of the ESP32 access
// point You will see a new Wi-Fi network with this
// name and password on your phone/PC's Wi-Fi settings
// when you run this code

// Enter your desired Wi-Fi SSID (network name) below
const char *ssid = "ESP32 Network 1";

// Enter your desired Wi-Fi password below
const char *password = "thecakeisalie";

// The IP configs of the ESP32 access point
// You don't have to change these
IPAddress localIpAddress(192, 168, 1, 1);
IPAddress gatewayIpAddress(192, 168, 1, 1);
IPAddress subnetMask(255, 255, 255, 0);

WebServer server(80);

String generateHtml()
{
    // Makes a String with the default HTML code on the webpage
    String html = "";

    html += "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<title>ESP32 Data</title>";
    html += "<meta http-equiv='refresh' content='2'>";
    html += "</head>";
    html += "<body>";

    html += "<h1>Data:</h1>";

    html += "<h2>Motor Speed: ";
    html += String(motorSpeed);
    html += "</h2>";

    // The fetch(url) is a javascript function that sends a http get request
    // When the buttons are pressed, the fetch function will send a http get request
    // to the path specified in void setup() that triggers the arduino functions
    // handleSpeedUpPath() and handleSpeedDownPath()
    // The javascript function location.reload() will refresh the website when the
    // ESP32 responds to the request with the status code 200, which is done with server.send(200);
    html += "<button onClick=\"fetch(location.href + 'speed-up').then(()=>location.reload())\">Speed Up</button>";
    html += "<button onClick=\"fetch(location.href + 'speed-down').then(()=>location.reload())\">Speed Down</button>";

    html += "</body>";
    html += "</html>";

    return html;
}

///////////////////////////////////////////////////////////////
//////////////////// Handle URL paths /////////////////////////
///////////////////////////////////////////////////////////////

// Code to run when the client (browser) requests the website
// and the path is /
void handleRootPath()
{
    Serial.println("A web browser requested the website");

    String html = generateHtml();

    // Sends the html string to the client (browser)
    // as a response to the http get request that triggered
    // this function to run
    // 200 is the status code which means everything is ok
    // and the request was successful
    // "text/html" is the content type which means the
    // response is html code
    server.send(200, "text/html", html);
}

// This function is run when one of the
// speed up button on the website is clicked
void handleSpeedUpPath()
{
    motorSpeed += 1;
    Serial.println("Increasing motor speed: " + String(motorSpeed));

    // Respond to the http request with the status code 200
    // which means everything is ok and the request was
    // successful
    server.send(200);
}

// This function is run when one of the
// speed down button on the website is clicked
void handleSpeedDownPath()
{
    motorSpeed -= 1;
    Serial.println("Decreasing motor speed: " + String(motorSpeed));

    // Respond to the http request with the status code 200
    // which means everything is ok and the request was
    // successful
    server.send(200);
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

void setup()
{
    Serial.begin(115200);

    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(localIpAddress, gatewayIpAddress, subnetMask);

    Serial.print("Connect to My access point: ");
    Serial.println(ssid);

    // Once the client (web browser) opens the ESP32 website
    // on one of these paths, the function specified will run
    // "/"" is the only address that responds with a html website
    // "/speed-up" and "/speed-down" are called from the buttons
    // in the website and will simply increase and decrease the
    // motor speed variable
    // To test this, open the URL 192.168.1.1/ in your browser to see
    // the website.
    // Open the URL 192.168.1.1/speed-up in a different tab and
    // see the motor speed change on the website in the first tab
    // When you press the speed up button, it is as if you opened
    // the speed-up URL in a different tab.
    server.on("/", handleRootPath);
    server.on("/speed-up", handleSpeedUpPath);
    server.on("/speed-down", handleSpeedDownPath);

    server.begin();
    Serial.println("HTTP server started");
    delay(100);
}

void loop()
{
    server.handleClient();
}