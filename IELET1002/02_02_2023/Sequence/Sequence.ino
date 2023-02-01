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

    void reset()
    {
        step = 0;
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

    void goToOnCondition(int newStep, bool condition)
    {
        if (condition)
        {
            goTo(newStep);
        }
    }

    void goToAfterDelay(int newStep, unsigned long duration)
    {
        if (timer.isFinished(duration))
        {
            goTo(newStep);
        }
    }

    void next()
    {
        step++;
        // Serial.println("Moving to step " + String(step));
        timer.reset();
    }

    void previous()
    {
        step--;
        timer.reset();
    }

    void goTo(int newStep)
    {
        step = newStep;
        timer.reset();
    }
};

struct Button
{
    int pin;
    // true for pulldown, false for pullup
    bool pulldown;

    bool state = false;
    bool prevState = false;
    bool pressed = false;
    bool released = false;
    int timesPressed = 0;

    void update()
    {
        if (pulldown)
            state = digitalRead(pin);
        else
            state = !digitalRead(pin);

        pressed = state && !prevState;
        released = !state && prevState;
        prevState = state;

        if (pressed)
        {
            timesPressed++;
        }
    }
};

Button upButton;
Button downButton;
Sequence sequence;

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
        sequence.nextAfterDelay(2000);
        break;
    case 2:
        Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        Serial.println("0_________1_________2_________3");
        Serial.println("          A");
        Serial.println("          |");

        sequence.next();
        break;
    case 3:
        sequence.nextAfterDelay(2000);
        break;
    case 4:
        Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        Serial.println("0_________1_________2_________3");
        Serial.println("                    A");
        Serial.println("                    |");

        sequence.next();
        break;
    case 5:
        sequence.nextAfterDelay(2000);
        break;
    case 6:
        Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        Serial.println("0_________1_________2_________3");
        Serial.println("                              A");
        Serial.println("                              |");

        sequence.next();
        break;
    case 7:
        sequence.nextAfterDelay(2000);
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

    upButton.pin = 2;
    upButton.pulldown = false;
    pinMode(upButton.pin, INPUT);

    downButton.pin = 3;
    downButton.pulldown = false;
    pinMode(downButton.pin, INPUT);
}

void loop()
{
    upButton.update();
    downButton.update();

    updateStateMachine();
    updateGuiControls();
}