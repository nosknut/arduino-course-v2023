struct Led
{
    int pin;
    bool state = false;

    void setState(bool newState)
    {
        state = newState;
        digitalWrite(pin, state);
    }

    void toggle()
    {
        setState(!state);
    }
};
