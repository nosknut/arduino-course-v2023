#ifndef LED_h
#define LED_h

struct Led
{
    int pin = 0;
    bool state = false;

    void on()
    {
        set(true);
    }

    void off()
    {
        set(false);
    }

    void set(bool newState)
    {
        state = newState;
        analogWrite(pin, state);
    }

    void setAnalog(int value)
    {
        analogWrite(pin, value);
    }

    void toggle()
    {
        set(!state);
    }

    void setup()
    {
        pinMode(pin, OUTPUT);
    }
};

#endif
