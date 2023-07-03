#ifndef WINNER_CELEBRATION_h
#define WINNER_CELEBRATION_h

#include "Led.h"
#include "Rgb.h"
#include "WinnerLed.h"
#include "LooserLed.h"
#include "Buzzer.h"

struct WinnerCelebration
{
    bool active = false;
    unsigned long timer = 0;

    void start(
        int winnerPlayer,
        WinnerLed &winnerLed,
        Buzzer &buzzer)
    {
        // Start the winner celebration
        active = true;
        timer = millis();
        winnerLed.start(winnerPlayer);
        buzzer.startWinnerCelebration();
    }

    // Stops the winner celebration after 3 seconds
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

        if ((millis() - timer) < 3000)
        {
            // Exit the function until 3 seconds have passed
            return;
        }

        // Stop the winner celebration
        active = false;

        winnerLed.stop(led1, led2, rgb);
        looserLed.stop(led1, led2, rgb);
        buzzer.stop();
    }
};

#endif
