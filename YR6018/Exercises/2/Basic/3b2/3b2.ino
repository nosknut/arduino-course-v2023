/////////////////////////////////////
/////////// Button Code /////////////
/////////////////////////////////////

bool buttonState = false;

const int BUTTON_PIN = 2;

void updateButton()
{
    buttonState = digitalRead(BUTTON_PIN);
}

void setupButton()
{
    pinMode(BUTTON_PIN, INPUT);
}

/////////////////////////////////////
///////////// LED Code //////////////
/////////////////////////////////////

const int LED_PIN = 5;

bool ledState = false;
unsigned long ledTimer = 0;

void updateLed()
{
    if (buttonState)
    {
        ledState = true;
    }
    else
    {
        // Deactivate the LED when the given duration
        // has passed since the button was released
        if ((millis() - ledTimer) > 500)
        {
            ledState = !ledState;
            ledTimer = millis();
        }
    }

    digitalWrite(LED_PIN, ledState);
}

void setupLed()
{
    pinMode(LED_PIN, OUTPUT);
}

/////////////////////////////////////
/////////// Main Code ///////////////
/////////////////////////////////////

void setup()
{
    Serial.begin(9600);

    setupButton();
    setupLed();
}

void loop()
{
    updateButton();
    updateLed();
}
