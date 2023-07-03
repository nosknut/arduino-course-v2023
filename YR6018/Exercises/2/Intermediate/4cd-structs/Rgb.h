#ifndef RGB_H
#define RGB_H

#include <Arduino.h>
#include "getSineWave.h"

enum class RgbMode
{
    OFF,
    PULSE,
    GREEN,
    PULSE_GREEN,
    PULSE_YELLOW_ORANGE,
    CYCLE
};

struct Rgb
{
    int redPin = 0;
    int greenPin = 0;
    int bluePin = 0;

    RgbMode mode = RgbMode::OFF;

    void setMode(RgbMode newMode)
    {
        mode = newMode;
    }

    void update()
    {
        switch (mode)
        {
        case RgbMode::OFF:
            analogWrite(redPin, 0);
            analogWrite(greenPin, 0);
            analogWrite(bluePin, 0);
            break;
        case RgbMode::GREEN:
            analogWrite(redPin, 0);
            analogWrite(greenPin, 255);
            analogWrite(bluePin, 0);
            break;
        case RgbMode::PULSE:
            // A quick google search shows that yellow is (R=255, G=255, B=0)
            // To pulse between red and yellow we just have to pulse the green color
            // https://www.google.com/search?q=rgb+values+for+yellow
            // After experimenting with the simulation,
            // the best value to produce orange is (R=255, G=40, B=0)

            analogWrite(redPin, 255);
            analogWrite(greenPin, getSineWave(20.0, 0.5, 20.0, 0.0));
            analogWrite(bluePin, 0);
            break;
        case RgbMode::PULSE_YELLOW_ORANGE:
            analogWrite(redPin, 255);
            // Yellow is ~60 and orange is ~20 = 40+-20
            analogWrite(greenPin, getSineWave(20.0, 0.5, 40.0, 0.0));
            analogWrite(bluePin, 0);
            break;
        case RgbMode::PULSE_GREEN:
            analogWrite(redPin, 0);
            // Pulsing at 100+-100
            analogWrite(greenPin, getSineWave(100.0, 1.0, 100.0, 0.0));
            analogWrite(bluePin, 0);
            break;
        case RgbMode::CYCLE:
            // The sine waves look like this: https://www.tinkercad.com/things/8wqREUCNY4p
            analogWrite(redPin, getSineWave(255.0, 1.0, 0.0, 0.0));
            analogWrite(greenPin, getSineWave(255.0, 1.0, 0.0, 120.0));
            analogWrite(bluePin, getSineWave(255.0, 1.0, 0.0, 240.0));
            break;
        }
    }

    void setup()
    {
        pinMode(redPin, OUTPUT);
        pinMode(greenPin, OUTPUT);
        pinMode(bluePin, OUTPUT);
    }
};

#endif