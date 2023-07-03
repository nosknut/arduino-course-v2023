#ifndef WINNER_LED_h
#define WINNER_LED_h

#include "Led.h"
#include "Rgb.h"
#include "getSquareWave.h"
#include "getTriangleWave.h"

struct WinnerLed
{
    // Creating a dummy led to be used later
    int winnerPlayerIndex = 0;
    bool run = false;

    void start(int winnerPlayer)
    {
        winnerPlayerIndex = winnerPlayer;
        run = true;
    }

    void stop(Led &led1, Led &led2, Rgb &rgb)
    {
        run = false;
        rgb.off();
        led1.off();
        led2.off();
    }

    // Rapidly fades the winner LED in and out
    void update(Led &led1, Led &led2, Rgb &rgb)
    {
        if (!run)
        {
            // Exit the function
            return;
        }

        int ledValue = getTriangleWave(1, 255, 0);

        switch (winnerPlayerIndex)
        {
        case 1:
            led1.setAnalog(ledValue);
            break;
        case 2:
            led2.setAnalog(ledValue);
            break;
        }

        rgb.set(0, getSquareWave(1, 1, 0), 0);
    }
};

#endif
