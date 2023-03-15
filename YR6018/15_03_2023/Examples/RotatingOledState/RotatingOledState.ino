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

enum class OledState {
    TEMPERATURE,
    HUMIDITY,
    PRESSURE,
    POTMETER,
};


OledState state = OledState::TEMPERATURE;
Timer timer1;
Button button1;

OledState getNextOledState(OledState state) {
    switch (state) {
        case OledState::TEMPERATURE:
            return OledState::HUMIDITY;
        case OledState::HUMIDITY:
            return OledState::PRESSURE;
        case OledState::PRESSURE:
            return OledState::POTMETER;
        case OledState::POTMETER:
            return OledState::TEMPERATURE;
    }
}

void printOledState() {
    switch(state) {
        case OledState::TEMPERATURE:
            Serial.println("Temperature");
            break;
        case OledState::HUMIDITY:
            Serial.println("Humidity");
            break;
        case OledState::PRESSURE:
            Serial.println("Pressure");
            break;
        case OledState::POTMETER:
            Serial.println("Potmeter");
            break;
    }
}

void updateOledState() {
    if (button1.pressed)
    {
        state = getNextOledState(state);
    }

    if (timer1.isFinished(3000)) {
        timer1.reset();
        state = getNextOledState(state);
    }
}

void setupButtons() {
    button1.pin = 19;
    button1.pulldown = false;
    pinMode(button1.pin, INPUT_PULLUP);
}

void setup()
{
    Serial.begin(9600);

    setupButtons();
}

void loop() {
    button1.update();

    updateOledState();
    printOledState();
}
