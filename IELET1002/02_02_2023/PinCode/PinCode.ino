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

struct Potmeter
{
    int pin;
    int value = 0;
    int number = 0;
    bool numberChanged = false;

    void update()
    {
        int newValue = analogRead(pin);

        float number = map(value, 0, 1023, 0, 9);
        int roundedNumber = floor(number);

        if (number != roundedNumber)
        {
            numberChanged = true;
        }
        else
        {
            numberChanged = false;
        }
    }
};

struct PinCodeStorage
{
    int selectedDigit = 0;

    int digit0 = 0;
    int digit1 = 0;
    int digit2 = 0;
    int digit3 = 0;

    void resetPin()
    {
        digit0 = -1;
        digit1 = -1;
        digit2 = -1;
        digit3 = -1;

        printDigits();
    }

    void selectDigit(int digit)
    {
        selectedDigit = digit;
    }

    void setDigitValue(int value)
    {
        switch (selectedDigit)
        {
        case 0:
            digit0 = value;
            break;
        case 1:
            digit1 = value;
            break;
        case 2:
            digit2 = value;
            break;
        case 3:
            digit3 = value;
            break;
        }

        printDigits();
    }

    String digitValueToString(int value)
    {
        if (value == -1)
        {
            return " ";
        }
        else
        {
            return String(value);
        }
    }

    void printDigits()
    {
        Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        Serial.print(digitValueToString(digit0));
        Serial.print("_________");
        Serial.print(digitValueToString(digit1));
        Serial.print("_________");
        Serial.print(digitValueToString(digit2));
        Serial.print("_________");
        Serial.println(digitValueToString(digit3));

        switch (selectedDigit)
        {
        case 0:
            Serial.println("A");
            Serial.println("|");
            break;
        case 1:
            Serial.println("          A");
            Serial.println("          |");
            break;
        case 2:
            Serial.println("                    A");
            Serial.println("                    |");
            break;
        case 3:
            Serial.println("                              A");
            Serial.println("                              |");
            break;
        }
    }

    bool verifyPin(int pin0, int pin1, int pin2, int pin3)
    {
        if (digit0 != pin0)
        {
            return false;
        }

        if (digit1 != pin1)
        {
            return false;
        }

        if (digit2 != pin2)
        {
            return false;
        }

        if (digit3 != pin3)
        {
            return false;
        }

        return true;
    }
};

enum class PinMode
{
    CHANGE_PIN,
    INPUT_PIN,
};

PinMode pinMode = PinMode::CHANGE_PIN;

Potmeter potmeter;

PinCodeSafe storedPinCode;
PinCodeSafe enteredPinCode;

Sequence enterPinSequence;
Sequence changePinSequence;

void updateStoredDigitInput()
{
    if (potmeter.numberChanged)
    {
        storedPinCode.setDigit(potmeter.number);
        changePinSequence.timer.reset();
    }
}

void updateEnteredDigitInput()
{
    if (potmeter.numberChanged)
    {
        enteredPinCode.setDigit(potmeter.number);
        enterPinSequence.timer.reset();
    }
}

void updateChangePinSequence()
{
    switch (changePinSequence.step)
    {
    case 0:
        // Normal state. Do nothing.
        break;
    case 1:
        storedPinCode.selectDigit(0);
        changePinSequence.next();
        break;
    case 2:
        updateStoredDigitInput();
        changePinSequence.nextAfterDelay(3000);
        break;
    case 3:
        storedPinCode.selectDigit(1);
        changePinSequence.next();
        break;
    case 4:
        updateStoredDigitInput();
        changePinSequence.nextAfterDelay(3000);
        break;
    case 5:
        storedPinCode.selectDigit(2);
        changePinSequence.next();
        break;
    case 6:
        updateStoredDigitInput();
        changePinSequence.nextAfterDelay(3000);
        break;
    case 7:
        storedPinCode.selectDigit(3);
        changePinSequence.next();
        break;
    case 8:
        updateStoredDigitInput();
        changePinSequence.nextAfterDelay(3000);
        break;
    case 9:
        Serial.println("Pin changed!");
        pinMode = PinMode::INPUT_PIN;
        changePinSequence.reset();
        break;
    }
}

void updateEnterPinSequence()
{
    switch (enterPinSequence.step)
    {
    case 0:
        // Normal state. Do nothing.
        break;
    case 1:
        enteredPinCode.selectDigit(0);
        enterPinSequence.next();
        break;
    case 2:
        updateEnteredDigitInput();
        enterPinSequence.nextAfterDelay(3000);
        break;
    case 3:
        enteredPinCode.selectDigit(1);
        enterPinSequence.next();
        break;
    case 4:
        updateEnteredDigitInput();
        enterPinSequence.nextAfterDelay(3000);
        break;
    case 5:
        enteredPinCode.selectDigit(2);
        enterPinSequence.next();
        break;
    case 6:
        updateEnteredDigitInput();
        enterPinSequence.nextAfterDelay(3000);
        break;
    case 7:
        enteredPinCode.selectDigit(3);
        enterPinSequence.next();
        break;
    case 8:
        updateEnteredDigitInput();
        enterPinSequence.nextAfterDelay(3000);
        break;
    case 9:
        if (
            storedPinCode.verifyPin(
                enteredPinCode.digit0,
                enteredPinCode.digit1,
                enteredPinCode.digit2,
                enteredPinCode.digit3))
        {
            Serial.println("Pin correct!");
            pinMode = PinMode::CHANGE_PIN;
            enterPinSequence.reset();
        }
        else
        {
            Serial.println("Pin incorrect!");
            enterPinSequence.reset();
        }
        break;
    }
}

void updateCurrentPinMode()
{
    switch (pinMode)
    {
    case PinMode::CHANGE_PIN:
        updateChangePinSequence();
        break;
    case PinMode::INPUT_PIN:
        updateEnterPinSequence();
        break;
    }
}

void setup()
{
    Serial.begin(9600);

    potmeter.pin = A0;
    pinMode(potmeter.pin, INPUT);

    upButton.pin = 2;
    upButton.pulldown = false;
    pinMode(upButton.pin, INPUT);

    downButton.pin = 3;
    downButton.pulldown = false;
    pinMode(downButton.pin, INPUT);
}

void loop()
{
    potmeter.update();

    updateCurrentPinMode();
}
