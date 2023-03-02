#include <WiFi.h>
#include <esp_now.h>
#include <ArduinoJson.h>

int motorSpeed = 0;

// Pins for the serial communication
const int RXD1 = 26;
const int TXD1 = 22;

void setup()
{
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, RXD1, TXD1);
}

void loop()
{
    resendSerial1();
}

DynamicJsonDocument createRemoteCommandJson(String command)
{
    DynamicJsonDocument jsonDoc(500);
    jsonDoc["sender"] = "webserver";
    jsonDoc["receiver"] = "fan";
    jsonDoc["topic"] = "remote-commands";
    jsonDoc["command"] = command;
    return jsonDoc;
}

void sendSpeedUpCommand()
{
    serializeJson(createRemoteCommandJson("speed-up"), Serial1);
}

void sendSpeedDownCommand()
{
    serializeJson(createRemoteCommandJson("speed-down"), Serial1);
}

void processReceivedJson(DynamicJsonDocument &jsonDoc)
{
    Serial.println("Received data:");

    // Converts the jsonDoc variable to text and sends it through
    // the Serial port using Serial.print() behind the scenes
    serializeJson(jsonDoc, Serial);
    Serial.println("");

    String topic = jsonDoc["topic"].as<String>();

    if (topic == "motor-speed")
    {
        motorSpeed = jsonDoc["motor-speed"].as<int>();
    }
}

void resendSerial1()
{
    if (Serial1.available() > 0)
    {
        DynamicJsonDocument receivedJson(500);
        DeserializationError error = deserializeJson(receivedJson, Serial1);

        if (error != DeserializationError::Ok)
        {
            Serial.print(F("deserializeJson() failed with the error message: "));
            Serial.println(error.c_str());
            return;
        }

        processReceivedJson(receivedJson);
    }
}
