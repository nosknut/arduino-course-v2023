#ifndef LOOSER_LED_h
#define LOOSER_LED_h

#include "Led.h"
#include "Rgb.h"
#include "getSquareWave.h"

struct LooserLed
{

    int looserPlayerIndex = 0;
    bool run = false;

    void start(int looserPlayer)
    {
        looserPlayerIndex = looserPlayer;
        run = true;
    }

    void stop(Led &led1, Led &led2, Rgb &rgb)
    {
        run = false;
        rgb.off();
        led1.off();
        led2.off();
    }

    // Rapidly flashes the looser LED
    void update(Led &led1, Led &led2, Rgb &rgb)
    {
        if (!run)
        {
            // Exit the function
            return;
        }

        int ledValue = getSquareWave(4, 255, 0);

        switch (looserPlayerIndex)
        {
        case 1:
            led1.set(ledValue);
            break;
        case 2:
            led2.set(ledValue);
            break;
        }

        rgb.set(getSquareWave(4, 1, 0), 0, 0);
    }
};

#endif
