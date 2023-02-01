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

struct Sequence
{
    int step = 0;

    void next()
    {
        step += 1;
    }

    void reset()
    {
        step = 0;
    }
};

Sequence sequence1;

Timer timer1;
Button button1;

void setup()
{
    Serial.begin(9600);

    button1.pin = 2;
    button1.pulldown = true;
    pinMode(button1.pin, INPUT);
}

void loop()
{
    button1.update();

    if (button1.pressed)
    {
        sequence1.next();
    }

    switch (sequence1.step)
    {
    case 0:
        Serial.println("Step 0");
        sequence1.next();
        break;
    case 1:
        // Do nothing
        break;
    case 2:
        Serial.println("Step 1");
        sequence1.next();
    case 3:
        // Do nothing
        break;
    default:
        sequence1.reset();
        break;
    }

    delay(100);
}
