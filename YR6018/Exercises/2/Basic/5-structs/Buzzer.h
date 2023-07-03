#ifndef BUZZER_h
#define BUZZER_h

#include "getSquareWave.h"
#include "getTriangleWave.h"

struct Buzzer
{
    int pin = 0;

    bool playWinnerCelebration = false;
    bool playLooserHumiliation = false;

    void stop()
    {
        playWinnerCelebration = false;
        playLooserHumiliation = false;
        noTone(pin);
    }

    void startWinnerCelebration()
    {
        stop();
        playWinnerCelebration = true;
    }

    void startLooserHumiliation()
    {
        stop();
        playLooserHumiliation = true;
    }

    // Plays the winner celebration buzz
    // when enabled.
    // Do not call this function directly.
    // Use update() instead.
    void updateWinnerCelebration()
    {
        if (playWinnerCelebration)
        {
            // The code below uses the same logic as the LED code.
            // The frequency of the tone
            // changes from 500 Hz to (500 + 1500 = 2000) Hz and back to 500 Hz
            // 2 times per second in a triangular pattern.
            // https://www.statisticshowto.com/wp-content/uploads/2019/12/triangle-wave-function.png
            tone(pin, getTriangleWave(2, 1500, 0) + 500);
        }
    }

    // Plays the looser humiliation buzz
    // when enabled.
    // Do not call this function directly.
    // Use update() instead.
    void updateLooserHumiliation()
    {
        if (playLooserHumiliation)
        {
            // The code below uses the same logic as the LED code.
            // The frequency of the tone
            // switches between 100 Hz and (100 + 100 = 200) Hz
            // 4 times per second in a square pattern.
            // https://www.statisticshowto.com/wp-content/uploads/2019/12/square-wave-function.png
            tone(pin, getSquareWave(4, 100, 0) + 100);
        }
    }

    // Call this function in void loop()
    // to update the tones for the .
    void update()
    {
        updateWinnerCelebration();
        updateLooserHumiliation();
    }

    void setup()
    {
        pinMode(pin, OUTPUT);
    }
};

#endif
