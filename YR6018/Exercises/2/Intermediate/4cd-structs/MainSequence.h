#ifndef MAIN_SEQUENCE_H
#define MAIN_SEQUENCE_H

#include <Arduino.h>
#include "CatDX4001.h"
#include "Button.h"
#include "Buzzer.h"
#include "Hatch.h"
#include "LedIndicator.h"
#include "Rgb.h"
#include "ServoPositionController.h"
#include "Thermometer.h"
#include "LightSensor.h"
#include "DryBlendSequence.h"

struct MainSequence
{
    int step = 0;

    unsigned long case1LightTimer = 0;

    unsigned long case2BuzzTimer = 0;
    unsigned long case2BatchDestructionTimer = 0;

    void update(
        Buzzer &buzzer,
        LedIndicator &ledIndicator,
        Button &buttonA,
        CatDX4001 &cat,
        LightSensor &lightSensor,
        Hatch &hatch,
        DryBlendSequence &dryBlendSequence,
        Thermometer &thermometer)
    {
        switch (step)
        {
        //////////////////
        ///// Case 1 /////
        //////////////////
        case 0:
            // Code that should run once during case 1
            ledIndicator.setMode(LedIndicatorMode::CASE_1);
            buzzer.playCaseChange();

            Serial.println("Case 1 is active");
            Serial.println("Press the confirmation button to continue");
            step += 1;
            break;
        case 1:
            if (buttonA.risingEdge)
            {
                step += 1;
            }
            break;
        case 2:
            if (cat.get_status())
            {
                step += 1;
            }
            break;
        case 3:
            if (cat.select_operation(1))
            {
                step += 1;
            }
            break;
        case 4:
            if (lightSensor.lightLevel > 50)
            {
                Serial.println("Waiting for the light level to get below 50");
            }
            step += 1;
            break;
        case 5:
            if (lightSensor.lightLevel < 50)
            {
                step += 1;
            }
            break;
        case 6:
            Serial.println("Waiting 2 minutes for the batch settle");
            case1LightTimer = millis();
            step += 1;
            break;
        case 7:
        {
            // Reset the sequence to the start of the light level check
            if (lightSensor.lightLevel > 50)
            {
                step = 4;
            }

            // Continuing to the next step after 2 minutes
            if ((millis() - case1LightTimer) > (2 * 60e3))
            {
                step += 1;
            }
            break;
        }
        //////////////////
        ///// Case 2 /////
        //////////////////
        case 8:
            // Code that should run once during case 2
            ledIndicator.setMode(LedIndicatorMode::CASE_2);
            buzzer.playCaseChange();

            Serial.println("Case 2 is active");
            Serial.println("Waiting for temperature to rise above 25 degrees");
            step += 1;
            break;
        case 9:
            if (thermometer.temperature > 25)
            {
                Serial.println("Temperature is above 25 degrees");
                Serial.println("Press the confirmation button within 2 minutes or the batch will be destroyed");
                step += 1;
            }
            break;
        case 10:
            case2BuzzTimer = millis();
            case2BatchDestructionTimer = millis();
            step += 1;
            break;
        case 11:
        {
            // The code in this case is surrounded by curly braces {} to
            // create a new scope. This allows for the creation of local variables
            // inside the case.

            if (buttonA.risingEdge)
            {
                buzzer.stop();
                step += 1;
            }

            unsigned long batchAge = millis() - case2BatchDestructionTimer;
            unsigned long timeSinceLastBuzz = millis() - case2BuzzTimer;

            // Using scientific notation to make the code more readable
            // 60e3 = 60 * 10^3 = 60 * 1000 = 60000 where 3 stands for 3 zeros (1000)
            if (batchAge <= 60e3)
            {
                if ((millis() - case2BuzzTimer) >= 10e3)
                {
                    buzzer.playCase2Short();
                    case2BuzzTimer = millis();
                }
            }
            else if (batchAge <= (60e3 + 30e3))
            {
                if (timeSinceLastBuzz >= 5e3)
                {
                    buzzer.playCase2Short(2);
                    case2BuzzTimer = millis();
                }
            }
            else if (batchAge <= (60e3 + 30e3 + 15e3))
            {
                if (timeSinceLastBuzz >= 5e3)
                {
                    buzzer.playCase2Short(3);
                    case2BuzzTimer = millis();
                }
            }
            else if (batchAge <= (2 * 60e3))
            {
                buzzer.playCase2Constant();
            }
            else
            {
                step = 0;
                buzzer.stop();
                Serial.println("Batch is destroyed!");
            }
            break;
        }
        //////////////////
        ///// Case 3 /////
        //////////////////
        case 12:
            // Code that should run once during case 3
            ledIndicator.setMode(LedIndicatorMode::CASE_3);
            buzzer.playCaseChange();

            Serial.println("Case 3 is active");

            hatch.startHatchSequence();

            step += 1;
            break;
        case 13:
        {
            // Code that should run over and over again while
            // waiting for the hatch sequence to complete

            // The code in this case is surrounded by curly braces {} to
            // create a new scope. This allows for the creation of local variables
            // inside the case.
            bool hatchSequenceCompleted = hatch.isClosed();

            // The hatch sequence gets updated by updateHatchSequence() in void loop()
            if (hatchSequenceCompleted)
            {
                Serial.println("Press the confirmation button to continue to step 4");
                step += 1;
            }
            break;
        }
        case 14:
            // Code that should run while waiting for the confirmation button press
            if (buttonA.risingEdge)
            {
                step += 1;
            }
            break;
        //////////////////
        ///// Case 4 /////
        //////////////////
        case 15:
            ledIndicator.setMode(LedIndicatorMode::CASE_4);
            buzzer.playCaseChange();

            Serial.println("Case 4 is active");
            dryBlendSequence.start();

            step += 1;
            break;
        case 16:
            if (!dryBlendSequence.active)
            {
                Serial.println("The batch has been completed!");
                Serial.println("Press the confirm button to start a new batch");
                step += 1;
            }
            break;
        case 17:
            if (buttonA.risingEdge)
            {
                step = 0;
            }
            break;
        }
    }
};

#endif