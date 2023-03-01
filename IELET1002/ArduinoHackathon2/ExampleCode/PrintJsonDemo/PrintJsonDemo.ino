#include <ArduinoJson.h>

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    delay(5000);

    // Create a variable of the data type DynamicJsonDocument that has a
    // capacity of 500 bytes (probably enough for our purposes)
    DynamicJsonDocument jsonDoc(500);

    // Add values to the json variable
    jsonDoc["sender"] = "themometer";
    jsonDoc["receiver"] = "webserver";
    jsonDoc["topic"] = "thermometer-sensor-data";
    jsonDoc["temperature"] = 2;

    // Converts the jsonDoc variable to text and saves
    // it in the jsonText variable
    String jsonText;
    serializeJson(jsonDoc, jsonText);

    // Prints the jsonText variable to the Serial port
    Serial.println(jsonText);

    delay(3000);

    // Alternatively this can be done without the string
    serializeJson(jsonDoc, Serial);
}
