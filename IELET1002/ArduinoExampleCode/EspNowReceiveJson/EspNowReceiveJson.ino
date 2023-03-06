#include <WiFi.h>
#include <esp_now.h>
#include <ArduinoJson.h>

int motorState = 0;

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

    // Register a function that will be called when data is received
    esp_now_register_recv_cb(onDataReceived);
}

void setup()
{
    Serial.begin(115200);
    setupEspNow();
}

void loop()
{
}