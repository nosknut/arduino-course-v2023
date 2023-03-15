#ifndef Oled_h
#define Oled_h

#include "OledState.h";
#include "Timer.h";

struct Oled
{
    OledState state = OledState::TEMPERATURE;

    void nextState()
    {
        switch (state)
        {
        case OledState::TEMPERATURE:
            state = OledState::HUMIDITY;
            break;
        case OledState::HUMIDITY:
            state = OledState::PRESSURE;
            break;
        case OledState::PRESSURE:
            state = OledState::POTMETER;
            break;
        case OledState::POTMETER:
            state = OledState::TEMPERATURE;
            break;
        }
    }
};

#endif