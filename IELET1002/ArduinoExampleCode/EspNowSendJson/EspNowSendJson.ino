#include <WiFi.h>
#include <esp_now.h>
#include <ArduinoJson.h>

// Declare an instance (variable with the datatype) of esp_now_peer_info_t
// that holds info about the ESP32 we want to send data to
// and receive data from in the MESH/Peer-to-Peer network.
esp_now_peer_info_t fanDeviceInfo = {
    // Change this address to the address of the other ESP32
    .peer_addr = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    .channel = 0,
    .encrypt = false,
};

void printEspErrorCode(String message, esp_err_t errorCode)
{
    Serial.println(message);
    Serial.print("Error Code:");
    Serial.println(errorCode, HEX);
    Serial.println("The meaning of this error code can be found at:");
    Serial.println("https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/error-codes.html");
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

    connectToEspNowPeer(fanDeviceInfo);

    // Once ESP-NOW is successfully initialized, we will register
    // a function that will be called when we successfully transmit data
    esp_now_register_send_cb(onDataTransmitted);
}

// Create a JSON document that contains the command that we want to send to the fan
DynamicJsonDocument createRemoteCommandJson(String command)
{
    DynamicJsonDocument jsonDoc(500);
    jsonDoc["sender"] = "remote";
    jsonDoc["receiver"] = "fan";
    jsonDoc["topic"] = "remote-commands";
    jsonDoc["command"] = command;
    return jsonDoc;
}

void setup()
{
    Serial.begin(115200);
    setupEspNow();
}

void loop()
{
    sendData(createRemoteCommandJson("speed-up"), fanDeviceInfo);
    delay(5000);

    sendData(createRemoteCommandJson("speed-down"), fanDeviceInfo);
    delay(5000);
}
