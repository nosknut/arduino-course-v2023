#include <ArduinoJson.h>

void printReceivedJson(DynamicJsonDocument &jsonDoc)
{
    // jsonDoc["sender"] grabs the value of the "sender" variable
    // inside the json message
    // .as<String>() converts the value to a string
    // .as<int>() converts the value to an integer
    // Values must be converted because everything
    // in json is stored as a string
    String sender = jsonDoc["sender"].as<String>();
    String receiver = jsonDoc["receiver"].as<String>();
    String topic = jsonDoc["topic"].as<String>();

    int temperature = jsonDoc["temperature"].as<int>();

    Serial.println("Received data:");

    Serial.println("Sender: " + sender);
    Serial.println("Receiver: " + receiver);
    Serial.println("Topic: " + topic);
    Serial.println("Temperature: " + String(temperature));
}

void setup()
{
    Serial.begin(115200);
}

void readSerial()
{
    // Is there data on the serial port?
    if (Serial.available() > 0)
    {
        // Create a variable of the data type DynamicJsonDocument that has a
        // capacity of 500 bytes (probably enough for our purposes)
        DynamicJsonDocument jsonDoc(500);

        // Read the string coming from the "Serial" port, find the variables
        // and save them in the jsonDoc variable
        DeserializationError error = deserializeJson(jsonDoc, Serial);

        // If there is an error, print it out
        // and return from (stop running) the function
        if (error != DeserializationError::Ok)
        {
            Serial.print(F("deserializeJson() failed with the error message: "));
            Serial.println(error.c_str());
            return;
        }

        printReceivedJson(jsonDoc);
    }
}

void loop()
{
    // Paste the json output from the PrintJsonDemo.ino into the Serial Monitor
    // and press enter
    // This program should print out the values of the variables
    readSerial();
}
