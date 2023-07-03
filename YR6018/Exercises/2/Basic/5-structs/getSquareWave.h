#ifndef SQUARE_WAVE_h
#define SQUARE_WAVE_h

/**
 * @param frequency The frequency of the square wave in Hz (Hertz).
 * @param amplitude The maximum value of the wave.
 * @param offset The offset of the square wave in ms (milliseconds).
 * @return A square wave with the given frequency and offset.
 */
long getSquareWave(double frequency, long amplitude, long offset)
{
    // The period of the square wave in ms (milliseconds)
    // the period is the time in ms (milliseconds) it takes for the
    // wave to complete one cycle (from 0 to 1 and back to 0)
    unsigned long period = 1000 / frequency;

    // The time since the program started + the offset
    // that skews the function in time.
    unsigned long time = millis() + offset;

    // https://www.arduino.cc/reference/en/language/structure/arithmetic-operators/remainder/
    // https://infosys.beckhoff.com/content/1033/tf5100_tc3_nc_i/Images/gif/3292507403__en-US__Web.gif
    double periodicTime = time % period;

    unsigned long halfPeriod = period / 2;

    // The square wave is 1 for the first half of the
    // period, and 0 for the second half of the period
    bool currentSquareWaveValue = periodicTime < halfPeriod;

    // Generate a value that is either equal to 0 or the amplitude
    if (currentSquareWaveValue == 1)
    {
        return amplitude;
    }
    else
    {
        return 0;
    }
}

#endif