#ifndef RGB_h
#define RGB_h

struct Rgb
{
    int redPin = 0;
    int greenPin = 0;
    int bluePin = 0;

    void set(bool red, bool green, bool blue)
    {
        digitalWrite(redPin, red);
        digitalWrite(greenPin, green);
        digitalWrite(bluePin, blue);
    }

    void off()
    {
        set(false, false, false);
    }

    void red()
    {
        set(true, false, false);
    }

    void green()
    {
        set(false, true, false);
    }

    void blue()
    {
        set(false, false, true);
    }

    void setup()
    {
        pinMode(redPin, OUTPUT);
        pinMode(greenPin, OUTPUT);
        pinMode(bluePin, OUTPUT);
    }
};

#endif