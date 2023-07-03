#ifndef SERVO_POSITION_CONTROLLER_H
#define SERVO_POSITION_CONTROLLER_H

#include <Arduino.h>

// The code in this struct slowly moves the
// servo to a target position.
struct ServoPositionController
{
    // Use this variable to set the desired servo position
    int target = 0;

    // Write this value to the servo in void loop()
    int position = 0;

    unsigned long timer = 0;
    unsigned long timerspeed = 20;

    // Moves the servo to target at a specific speed.
    // Call this function in void loop()
    void update()
    {
        if ((millis() - timer) < timerspeed)
        {
            // Exit the funtion until the timer finished
            return;
        }

        if (position > target)
        {
            position -= 1;
        }

        if (position < target)
        {
            position += 1;
        }

        timer = millis();
    }
};

#endif