#ifndef THERMOMETER_H
#define THERMOMETER_H

#include <Arduino.h>

struct Thermometer
{
    int pin = 0;

    int temperature = 0;

    // Reads the temperature from the thermometer and updates
    // the temperature variable.
    // Call this function in void loop()
    void update()
    {
        // Using a potentiometer to emulate the thermometer
        // Converting the potmeter value to a range of 0 to 30 degrees
        temperature = map(analogRead(pin), 0, 1024, 0, 30);
    }

    // Run this code in void setup()
    void setup()
    {
        pinMode(pin, INPUT);
    }
};

#endif