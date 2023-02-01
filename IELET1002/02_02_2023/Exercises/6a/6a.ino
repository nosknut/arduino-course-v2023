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
    Timer timer;

    void next()
    {
        step += 1;
        timer.reset();
    }

    void previous()
    {
        step -= 1;
        timer.reset();
    }

    void nextAfterDelay(unsigned long duration)
    {
        if (timer.isFinished(duration))
        {
            next();
        }
    }

    void nextOnCondition(bool condition)
    {
        if (condition)
        {
            next();
        }
    }

    void reset()
    {
        step = 0;
        timer.reset();
    }
};

Sequence sequence;

Timer timer1;
Button upButton;
Button downButton;

void updateStateMachine()
{
    switch (sequence.step)
    {
    case 0:
        Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        Serial.println("0_________1_________2_________3");
        Serial.println("A");
        Serial.println("|");

        sequence.next();
        break;
    case 1:
        break;
    case 2:
        Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        Serial.println("0_________1_________2_________3");
        Serial.println("          A");
        Serial.println("          |");

        sequence.next();
        break;
    case 3:
        break;
    case 4:
        Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        Serial.println("0_________1_________2_________3");
        Serial.println("                    A");
        Serial.println("                    |");

        sequence.next();
        break;
    case 5:
        break;
    case 6:
        Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        Serial.println("0_________1_________2_________3");
        Serial.println("                              A");
        Serial.println("                              |");

        sequence.next();
        break;
    case 7:
        break;
    case 8:
        sequence.reset();
        break;
    }
}

void updateGuiControls()
{
    if (upButton.pressed)
    {
        sequence.next();
    }

    if (downButton.pressed)
    {
        sequence.previous();
    }
}

void setup()
{
    Serial.begin(9600);

    upButton.pin = 3;
    upButton.pulldown = true;
    pinMode(upButton.pin, INPUT);

    downButton.pin = 4;
    downButton.pulldown = true;
    pinMode(downButton.pin, INPUT);
}

void loop()
{
    upButton.update();
    downButton.update();

    updateStateMachine();
    updateGuiControls();
    
    delay(100);
}
