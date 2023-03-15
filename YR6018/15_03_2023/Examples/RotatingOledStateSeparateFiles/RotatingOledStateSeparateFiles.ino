#include "OledState.h";
#include "Timer.h";
#include "Button.h";
#include "Oled.h";

Oled oled1;
Timer timer1;
Button button1;

void setupButtons()
{
    button1.pin = 19;
    button1.pulldown = false;
    pinMode(button1.pin, INPUT_PULLUP);
}

void updateOledState()
{
    if (button1.pressed)
    {
        oled1.nextState();
    }

    if (timer1.isFinished(3000))
    {
        timer1.reset();
        oled1.nextState();
    }
}

void printOledState()
{
    switch (oled1.state)
    {
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

void setup()
{
    Serial.begin(9600);
    
    setupButtons();
}

void loop()
{
    button1.update();

    updateOledState();
    printOledState();
}
