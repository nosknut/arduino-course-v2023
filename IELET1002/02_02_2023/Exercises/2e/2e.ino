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

Button button1;
Button button2;
Button button3;

void setup()
{
    Serial.begin(9600);

    button1.pin = 2;
    button1.pulldown = true;
    pinMode(button1.pin, INPUT);

    button2.pin = 4;
    button2.pulldown = true;
    pinMode(button2.pin, INPUT);

    button3.pin = 7;
    button3.pulldown = true;
    pinMode(button3.pin, INPUT);
}

void loop()
{
    button1.update();
    button2.update();
    button3.update();

    if (button1.pressed)
    {
        Serial.println("Button 1 pressed");
    }

    if (button2.pressed)
    {
        Serial.println("Button 2 pressed");
    }

    if (button3.pressed)
    {
        Serial.println("Button 3 pressed");
    }

    delay(100);
}
