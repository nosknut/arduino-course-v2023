#ifndef LOOSER_HUMILIATION_h
#define LOOSER_HUMILIATION_h

#include "Led.h"
#include "Rgb.h"
#include "WinnerLed.h"
#include "LooserLed.h"
#include "Buzzer.h"

struct LooserHumiliation
{

    bool active = false;
    unsigned long timer = 0;

    void start(
        int looserPlayer,
        LooserLed &looserLed,
        Buzzer &buzzer)
    {
        // Start the looser humiliation
        active = true;
        timer = millis();
        looserLed.start(looserPlayer);
        buzzer.startLooserHumiliation();
    }

    // Stops the looser humiliation after 4 seconds
    // Call this function in void loop()
    void update(
        WinnerLed &winnerLed,
        LooserLed &looserLed,
        Buzzer &buzzer,
        Rgb &rgb,
        Led &led1,
        Led &led2)
    {
        if (!active)
        {
            // Exit the function
            return;
        }

        if ((millis() - timer) < 4000)
        {
            // Exit the function until 4 seconds have passed
            return;
        }

        // Stop the looser humiliation
        active = false;
        winnerLed.stop(led1, led2, rgb);
        looserLed.stop(led1, led2, rgb);
        buzzer.stop();
    }
};

#endif
