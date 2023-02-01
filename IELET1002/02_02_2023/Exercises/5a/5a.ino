struct Button
{
    int pin;
    // true for pulldown, false for pullup
    bool pulldown;

    bool state = false;
    bool prevState = false;
    bool pressed = false;
    bool released = false;

    void update()
    {
        if (pulldown)
            state = digitalRead(pin);
        else
            state = !digitalRead(pin);

        pressed = state && !prevState;
        released = !state && prevState;
        prevState = state;
    }
};

struct Timer
{
    unsigned long startTime;

    void reset()
    {
        startTime = millis();
    }

    unsigned long getElapsedTime()
    {
        return millis() - startTime;
    }

    bool isFinished(unsigned long duration)
    {
        return getElapsedTime() >= duration;
    }
};

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

Led led1;
Timer timer1;
Button button1;

void setup()
{
    Serial.begin(9600);

    led1.pin = 3;
    pinMode(led1.pin, OUTPUT);

    button1.pin = 2;
    button1.pulldown = true;
    pinMode(button1.pin, INPUT);
}

void loop()
{
    button1.update();

    if (button1.pressed)
    {
        led1.toggle();
    }

    delay(100);
}
