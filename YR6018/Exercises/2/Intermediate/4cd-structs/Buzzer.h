#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>
#include "getSineWave.h"

struct Buzzer
{
    int pin = 0;

    unsigned long int case2CounterTimer = 0;
    int case2RemainingCounter = 0;

    bool dryBlendCompleteActive = false;

    void playCaseChange()
    {
        tone(pin, 500, 10);
    }

    void playHatchStep()
    {
        tone(pin, 1000, 100);
    }

    void playHatchClosed()
    {
        tone(pin, 250, 1000);
    }

    void playCase2Short()
    {
        tone(pin, 500, 100);
    }

    void playCase2Short(int buzzCount)
    {
        case2RemainingCounter = buzzCount;
    }

    void update()
    {
        if (case2RemainingCounter > 0)
        {
            if ((millis() - case2CounterTimer) >= 500)
            {
                playCase2Short();
                case2RemainingCounter--;
                case2CounterTimer = millis();
            }
        }

        if (dryBlendCompleteActive)
        {
            tone(pin, getSineWave(250.0, 3.0, 750.0, 0.0));
        }
    }

    void playCase2Constant()
    {
        tone(pin, 250);
    }

    void playDryBlendComplete()
    {
        dryBlendCompleteActive = true;
    }

    void stop()
    {
        dryBlendCompleteActive = false;
        noTone(pin);
    }

    void setup()
    {
        pinMode(pin, OUTPUT);
    }
};

#endif