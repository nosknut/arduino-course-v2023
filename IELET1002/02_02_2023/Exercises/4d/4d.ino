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

int step = 0;

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

    if (timer1.isFinished(1000))
    {
        timer1.reset();
        step += 1;
    }

    switch (step)
    {
    case 0:
        Serial.println("Step 0");
        step += 1;
        break;
    case 1:
        // Do nothing
        break;
    case 2:
        Serial.println("Step 1");
        step += 1;
    case 3:
        // Do nothing
        break;
    default:
        step = 0;
        break;
    }

    delay(100);
}
