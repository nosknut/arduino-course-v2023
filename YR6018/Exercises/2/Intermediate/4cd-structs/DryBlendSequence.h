#ifndef DRY_BLEND_SEQUENCE_H
#define DRY_BLEND_SEQUENCE_H

#include <Arduino.h>
#include "Buzzer.h"
#include "Rgb.h"

struct DryBlendSequence
{
    int active = false;

    int counter = 0;
    int step = 0;
    unsigned long timer = 0;

    void start()
    {
        active = true;
        step = 0;
    }

    bool isComplete()
    {
        return !active;
    }

    void update(Buzzer &buzzer, Rgb &rgb)
    {
        if (!active)
        {
            // Exit the function
            return;
        }

        switch (step)
        {
        //////////////////
        ////// Init //////
        //////////////////
        case 0:
            counter = 0;
            step += 1;
            break;
        //////////////////
        ///// Drying /////
        //////////////////
        case 1:
            counter += 1;
            Serial.println("Drying " + String(counter) + "/3 ...");
            rgb.setMode(RgbMode::PULSE);
            timer = millis();
            step += 1;
            break;
        case 2:
            if ((millis() - timer) >= 10e3)
            {
                step += 1;
            }
            break;
        //////////////////
        ///// Pause //////
        //////////////////
        case 3:
            Serial.println("Drying complete!");
            Serial.println("Waiting 13 seconds ...");
            rgb.setMode(RgbMode::PULSE_YELLOW_ORANGE);
            timer = millis();
            step += 1;
            break;
        case 4:
            if ((millis() - timer) >= 7e3)
            {
                step += 1;
            }
            break;
        case 5:
            rgb.setMode(RgbMode::PULSE_GREEN);
            timer = millis();
            step += 1;
            break;
        case 6:
            if ((millis() - timer) >= 3e3)
            {
                step += 1;
            }
            break;
        case 7:
            rgb.setMode(RgbMode::GREEN);
            timer = millis();
            step += 1;
            break;
        case 8:
            if ((millis() - timer) >= 3e3)
            {
                step += 1;
            }
            break;
        //////////////////
        /// Blending /////
        //////////////////
        case 9:
            Serial.println("Blending " + String(counter) + "/3 ...");
            rgb.setMode(RgbMode::CYCLE);
            timer = millis();
            step += 1;
            break;
        case 10:
            if ((millis() - timer) >= 5e3)
            {
                step += 1;
            }
            break;
        case 11:
            Serial.println("Blending complete!");
            if (counter < 3)
            {
                // Replay the sequence from the beginning 3 times
                step = 1;
            }
            else
            {
                // Continue the sequence after it
                // has completed its 3 runs
                step += 1;
            }
            break;
        //////////////////
        /// Play tone ////
        //////////////////
        case 12:
            Serial.println("Dry and Blend process complete!");
            rgb.setMode(RgbMode::OFF);
            buzzer.playDryBlendComplete();
            timer = millis();
            step += 1;
            break;
        case 13:
            if ((millis() - timer) >= 2e3)
            {
                step += 1;
            }
            break;
        //////////////////
        ////// End ///////
        //////////////////
        case 14:
            buzzer.stop();
            // Stop running the sequence
            active = false;
            break;
        }
    }
};

#endif