#ifndef GAME_SEQUENCE_h
#define GAME_SEQUENCE_h

#include "Rgb.h"
#include "WinnerLed.h"
#include "LooserLed.h"
#include "WinnerCelebration.h"
#include "LooserHumiliation.h"
#include "Buzzer.h"
#include "Button.h"

struct GameSequence
{
    int step = 0;
    unsigned long timer = 0;
    unsigned long randomDuration = 0;

    void update(
        Button &button1,
        Button &button2,
        WinnerLed &winnerLed,
        LooserLed &looserLed,
        WinnerCelebration &winnerCelebration,
        LooserHumiliation &looserHumiliation,
        Buzzer &buzzer,
        Rgb &rgb)
    {
        switch (step)
        {
        case 0:
            rgb.red();

            randomDuration = random(1000, 3000);
            timer = millis();
            step += 1;
            break;
        case 1:
            // During the random duration
            if ((millis() - timer) >= randomDuration)
            {
                step += 1;
            }

            if (button1.risingEdge)
            {
                looserHumiliation.start(1, looserLed, buzzer);
                step += 2;
            }

            if (button2.risingEdge)
            {
                looserHumiliation.start(2, looserLed, buzzer);
                step += 2;
            }
            break;
        // After the random duration
        case 2:
            rgb.green();

            if (button1.risingEdge)
            {
                winnerCelebration.start(1, winnerLed, buzzer);
                step += 1;
            }

            if (button2.risingEdge)
            {
                winnerCelebration.start(2, winnerLed, buzzer);
                step += 1;
            }
            break;
        case 3:
            // Wait for celebration and
            // humiliation to end
            if (!winnerCelebration.active && !looserHumiliation.active)
            {
                step = 0;
            }
            break;
        }
    }
};

#endif
