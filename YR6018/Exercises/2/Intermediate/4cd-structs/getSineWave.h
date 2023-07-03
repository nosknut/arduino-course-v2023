#ifndef SINE_WAVE_H
#define SINE_WAVE_H

#include <Arduino.h>

// https://www.wikihow.com/Convert-Degrees-to-Radians
double degreesToRadians(double degrees)
{
    return (degrees * PI) / 180.0;
}

// Generates a sine wave centered around the center value
// with a max=centerValue+amplitude and min=centerValue-amplitude
// The offset argument lets you add or remove time, and is
// useful if you need multiple sinewaves working together
// In 3 phase AC power for example there are 3 sine waves
// working at a 120 degree offset.
// https://en.wikipedia.org/wiki/Three-phase_electric_power#/media/File:3_phase_AC_waveform.svg
// https://www.google.com/search?q=generate+sine+wave+with+arduino
// Example: https://www.tinkercad.com/things/8wqREUCNY4p
double getSineWave(double amplitude, double frequency, double centerValue, double offset)
{
    // Creating a variable for the current moment in time
    // given in seconds. Using double as the datatype because
    // it can store massive numbers as decimals. The datatype
    // for the sin() function argument is also double.
    // https://reference.arduino.cc/reference/en/language/variables/data-types/double/
    double t = millis() / 1000.0;

    // How to use the sin() function: https://reference.arduino.cc/reference/tr/language/functions/trigonometry/sin/
    // The formula for a sine wave: https://en.wikipedia.org/wiki/Sine_wave
    return (amplitude * sin((2.0 * PI * frequency * t) + degreesToRadians(offset))) + centerValue;
}

#endif