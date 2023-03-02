#include <WiFi.h>
#include <esp_now.h>
#include <ArduinoJson.h>

int motorState = 0;

// Function declarations
// The functions are defined below the loop() function
// This is an overview of the functions that are defined in the code
void setupEspNow();
DynamicJsonDocument createMotorSpeedDataJson();
void connectToEspNowPeer(esp_now_peer_info_t &peerInfo);
void sendData(DynamicJsonDocument jsonDoc, esp_now_peer_info_t &peerInfo);
void onDataTransmitted(const uint8_t *receiverMacAddress, esp_now_send_status_t transmissionStatus);
void onDataReceived(const uint8_t *senderMacAddress, const uint8_t *receivedData, int receivedDataLength);
void processReceivedJson(DynamicJsonDocument &jsonDoc);
void printEspErrorCode(String message, esp_err_t errorCode);

// Declare an instance (variable with the datatype) of esp_now_peer_info_t
// that holds info about the ESP32 we want to send data to
// and receive data from in the MESH/Peer-to-Peer network.
esp_now_peer_info_t routerDeviceInfo = {
    // Change this address to the address of the other ESP32
    .peer_addr = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    .channel = 0,
    .encrypt = false,
};

void setup()
{
    Serial.begin(115200);
    setupEspNow();
}

void loop()
{
}

void setupEspNow()
{
    // Set device as a Wi-Fi Station because ESP-NOW
    // uses parts of the Wi-Fi protocol.
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    esp_err_t espNowInitStatus = esp_now_init();
    if (espNowInitStatus != ESP_OK)
    {
        // If the initialization fails, print a message and restart the ESP32
        printEspErrorCode("Error initializing ESP-NOW", espNowInitStatus);
        ESP.restart();
    }

    connectToEspNowPeer(routerDeviceInfo);

    // Once ESP-NOW is successfully initialized, we will register
    // a function that will be called when we successfully transmit data
    esp_now_register_send_cb(onDataTransmitted);

    // Register a function that will be called when data is received
    esp_now_register_recv_cb(onDataReceived);
}

// Create a json document with the sensor data
DynamicJsonDocument createMotorSpeedDataJson()
{
    DynamicJsonDocument jsonDoc(500);
    jsonDoc["sender"] = "fan";
    jsonDoc["receiver"] = "webserver";
    jsonDoc["topic"] = "motor-speed";
    jsonDoc["motor-speed"] = motorState;
    return jsonDoc;
}

void connectToEspNowPeer(esp_now_peer_info_t &peerInfo)
{
    // Avoid people forgetting to change the peer mac address
    if (peerInfo.peer_addr[0] == 0xFF)
    {
        Serial.println("Please change the peer mac address to the address of the other ESP32 at the top of the code");
        Serial.println("Rebooting");
        delay(5000);
        ESP.restart();
    }

    // Add peer (other ESP32)
    esp_err_t peerConnectionStatus = esp_now_add_peer(&peerInfo);
    if (peerConnectionStatus != ESP_OK)
    {
        printEspErrorCode("Failed to add peer", peerConnectionStatus);
        ESP.restart();
    }
}

void sendData(DynamicJsonDocument jsonDoc, esp_now_peer_info_t &peerInfo)
{
    // Create a string from jsonDoc
    // and store it in the payload variable
    String payload;
    int payloadSize = serializeJson(jsonDoc, payload);

    // Black magic that converts the string to a byte array
    // that can be transmitted with ESP-NOW
    uint8_t *payloadBytes = (uint8_t *)payload.c_str();

    esp_err_t status = esp_now_send(peerInfo.peer_addr, payloadBytes, payloadSize);

    if (status != ESP_OK)
    {
        printEspErrorCode("Unable to start transmission!", status);
        return;
    }
}

//  Callback function for when data is sent from this ESP32
//  The uint8_t is just a regular int with
// specified size, in this case a 8bit/1byte size
void onDataTransmitted(const uint8_t *receiverMacAddress, esp_now_send_status_t transmissionStatus)
{
    if (transmissionStatus != ESP_NOW_SEND_SUCCESS)
    {
        // esp_now_send_status_t is only a send status and does not contain any
        // useful error codes. That is why printEspErrorCode() is not used here.
        Serial.println("Data transmission failed!");
        return;
    }

    Serial.println("Data transmission was successful!");
}

// Callback function for when data is received from other ESP32
// The * and & symbols can mostly be ignored for our cases. Its arrays, references and pointers in the C-language used in specific cases.
void onDataReceived(const uint8_t *senderMacAddress, const uint8_t *receivedData, int receivedDataLength)
{
    DynamicJsonDocument receivedJson(500);

    // Read the received data and store it as json in the receiveJson variable
    DeserializationError error = deserializeJson(receivedJson, receivedData, receivedDataLength);

    // If there was an error converting the received data to json, print the error message
    // and exit the function
    if (error != DeserializationError::Ok)
    {
        Serial.print(F("deserializeJson() failed with the error message: "));
        Serial.println(error.c_str());
        return;
    }

    processReceivedJson(receivedJson);
}

void processReceivedJson(DynamicJsonDocument &jsonDoc)
{
    Serial.println("Received data:");

    // Converts the jsonDoc variable to text and sends it through
    // the Serial port using Serial.print() behind the scenes
    serializeJson(jsonDoc, Serial);
    Serial.println("");

    // Logic based on the command received
    String topic = jsonDoc["topic"].as<String>();
    if (topic == "remote-commands")
    {
        String command = jsonDoc["command"].as<String>();

        if (command == "speed-up")
        {
            Serial.println("Speeding up the fan");
            motorState += 1;
        }

        if (command == "speed-down")
        {
            Serial.println("Slowing down the fan");
            motorState -= 1;
        }

        sendData(createMotorSpeedDataJson(), routerDeviceInfo);
    }
}

void printEspErrorCode(String message, esp_err_t errorCode)
{
    Serial.println(message);
    Serial.print("Error Code:");
    Serial.println(errorCode, HEX);
    Serial.println("The meaning of this error code can be found at:");
    Serial.println("https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/error-codes.html");
}