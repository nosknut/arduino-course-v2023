#ifndef HATCH_H
#define HATCH_H

#include <Arduino.h>
#include "Buzzer.h"
#include <Servo.h>
#include "ServoPositionController.h"

enum class HatchStatus
{
    OPENING,
    CLOSING,
    OPEN,
    CLOSED
};

struct Hatch
{
    // This code contains the hatch opening and closing sequence.
    // The hatch code is split into a servo section and a sequence
    // secion because smoothly moving the servo and controlling the
    // hatch steps are different tasks that become much more complex
    // when combined. Now that these responsibilities are divided,
    // we can simply use the hatchServoTargetPosition variable to
    // request a specific servo position, and we can trust that
    // the servo section will take care of the details.

    // Indicates the status of the hatch sequence.
    // Do not change this variable directly.
    // Use openHatch() and closeHatch() instead.
    HatchStatus status = HatchStatus::CLOSED;

    unsigned long timer = 0;
    unsigned long stepDuration = 15000;

    int step = 0;

    void startHatchSequence()
    {
        status = HatchStatus::OPENING;
    }

    bool isClosed()
    {
        return status == HatchStatus::CLOSED;
    }

    // Opens and closes the hatch in steps of 10 degrees every 15 seconds.
    // Call this function in void loop()
    void updateHatchSequence(Buzzer &buzzer, ServoPositionController &servo)
    {
        switch (step)
        {
        //////////////////
        ////// Idle //////
        //////////////////
        case 0:
            if (status == HatchStatus::OPENING)
            {
                // Continue to the next step
                step += 1;
            }
            break;
        //////////////////
        /// Open Hatch ///
        //////////////////
        case 1:
            buzzer.playHatchStep();
            servo.target += 10;
            Serial.println("Moving the hatch to: " + String(servo.target) + " degrees");

            timer = millis();
            step += 1;
            break;
        case 2:
            if ((millis() - timer) >= stepDuration)
            {
                if (servo.position >= 90)
                {
                    // When the hatch is fully open, continue to the next step
                    step += 1;
                }
                else
                {
                    // If the hatch is not fully open, go back to the
                    // previous step and keep opening
                    step -= 1;
                }
            }
            break;
        case 3:
            servo.target = 90;
            status = HatchStatus::OPEN;
            Serial.println("Hatch is open!");
            timer = millis();
            step += 1;
            break;
        //////////////////
        /// Stay Open ////
        //////////////////
        case 4:
            if ((millis() - timer) >= stepDuration)
            {
                step += 1;
            }
            break;
        //////////////////
        /// Close Hatch //
        //////////////////
        case 5:
            Serial.println("Closing hatch ...");
            status = HatchStatus::CLOSING;
            step += 1;
            break;
        case 6:
            buzzer.playHatchStep();
            servo.target -= 10;
            Serial.println("Moving the hatch to: " + String(servo.target) + " degrees");

            timer = millis();
            step += 1;
            break;
        case 7:
            if ((millis() - timer) >= stepDuration)
            {
                if (servo.target <= 0)
                {
                    // When the hatch is fully closed, continue to the next step
                    step += 1;
                }
                else
                {
                    // If the hatch is not fully closed, go back to the
                    // previous step and keep closing
                    step -= 1;
                }
            }
            break;
        case 8:
            servo.target = 0;
            status = HatchStatus::CLOSED;
            Serial.println("Hatch is closed!");
            buzzer.playHatchClosed();
            step = 0;
            break;
        }
    }
};

#endif