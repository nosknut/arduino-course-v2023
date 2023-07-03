#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include <Arduino.h>

struct LightSensor
{
    int pin = 0;

    int lightLevel = 0;

    // Reads the light level from the light sensor and updates
    // the lightLevel variable.
    // Call this function in void loop()
    void update()
    {
        // Using a potentiometer to emulate the light sensor
        // Converting the potmeter value to a range of 0 to 100
        lightLevel = map(analogRead(pin), 0, 1024, 0, 100);
    }

    // Run this code in void setup()
    void setup()
    {
        pinMode(pin, INPUT);
    }
};

#endif