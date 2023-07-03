#ifndef LED_INDICATOR_H
#define LED_INDICATOR_H

#include <Arduino.h>

// Is used to make setting the state of the LED indicator more readable.
// Using enum class instead of enum because it is safer and groups the enum
// values under the enum name.
// In practice the difference between enum class and enum is the way it is used:
// LedIndicatorMode::CASE_1 instead of just CASE_1.
// If you are interested in the reasons why you should use one over the other, see the post below:
// https://www.geeksforgeeks.org/enum-classes-in-c-and-their-advantage-over-enum-datatype/
enum class LedIndicatorMode
{
    OFF,
    CASE_1,
    CASE_2,
    CASE_3,
    CASE_4
};

struct LedIndicator
{
    int led1Pin = 0;
    int led2Pin = 0;
    int led3Pin = 0;
    int led4Pin = 0;

    // Activates and deactivates the LEDS in the case LED
    // indicator based on the provided indicator mode.
    // Why use an enum for this instead of an int with the numbers 0 to 4?
    // Because it makes the code more readable.
    int setMode(LedIndicatorMode mode)
    {
        switch (mode)
        {
        case LedIndicatorMode::OFF:
            digitalWrite(led1Pin, LOW);
            digitalWrite(led2Pin, LOW);
            digitalWrite(led3Pin, LOW);
            digitalWrite(led4Pin, LOW);
            break;
        case LedIndicatorMode::CASE_1:
            digitalWrite(led1Pin, HIGH);
            digitalWrite(led2Pin, LOW);
            digitalWrite(led3Pin, LOW);
            digitalWrite(led4Pin, LOW);
            break;
        case LedIndicatorMode::CASE_2:
            digitalWrite(led1Pin, LOW);
            digitalWrite(led2Pin, HIGH);
            digitalWrite(led3Pin, LOW);
            digitalWrite(led4Pin, LOW);
            break;
        case LedIndicatorMode::CASE_3:
            digitalWrite(led1Pin, LOW);
            digitalWrite(led2Pin, LOW);
            digitalWrite(led3Pin, HIGH);
            digitalWrite(led4Pin, LOW);
            break;
        case LedIndicatorMode::CASE_4:
            digitalWrite(led1Pin, LOW);
            digitalWrite(led2Pin, LOW);
            digitalWrite(led3Pin, LOW);
            digitalWrite(led4Pin, HIGH);
            break;
        }
        return 1;
    }

    void setup()
    {
        pinMode(led1Pin, OUTPUT);
        pinMode(led2Pin, OUTPUT);
        pinMode(led3Pin, OUTPUT);
        pinMode(led4Pin, OUTPUT);
    }
};

#endif