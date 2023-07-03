#ifndef TRIANGLE_WAVE_h
#define TRIANGLE_WAVE_h

/**
 * Generates the following type of signal:
 * https://www.statisticshowto.com/wp-content/uploads/2019/12/triangle-wave-function.png
 * @param frequency The frequency of the wave in Hz (Hertz).
 * @param amplitude The maximum value of the wave.
 * @param offset The offset of the wave in ms (milliseconds).
 * @return A triangle wave with the given frequency and maximum value.
 */
unsigned long getTriangleWave(double frequency, long amplitude, long offset)
{
    // The period of the square wave in ms (milliseconds)
    // the period is the time in ms (milliseconds) it takes for the
    // wave to complete one cycle (from 0 to max and back to 0)
    unsigned long period = 1000 / frequency;

    // The time since the program started + the offset
    // that skews the function in time.
    unsigned long time = millis() + offset;

    // https://www.arduino.cc/reference/en/language/structure/arithmetic-operators/remainder/
    // https://infosys.beckhoff.com/content/1033/tf5100_tc3_nc_i/Images/gif/3292507403__en-US__Web.gif
    unsigned long periodicTime = time % period;

    unsigned long halfPeriod = period / 2;

    // The signal is 0 at the start of the period,
    // and increases linearly until half way through the period
    // where it is equal to the amplitude.
    // Then it decreases linearly until the end of the period
    // where it is equal to 0 again.
    if (periodicTime < halfPeriod)
    {
        // https://www.arduino.cc/reference/en/language/functions/math/map/
        return map(periodicTime, 0, halfPeriod, 0, amplitude);
    }
    else
    {
        // https://www.arduino.cc/reference/en/language/functions/math/map/
        return map(periodicTime, halfPeriod, period, amplitude, 0);
    }
}

#endif