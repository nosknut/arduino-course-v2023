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

unsigned long ledTimer = 0;

void updateLed()
{
    if (buttonState)
    {
        // Activate the LED
        digitalWrite(LED_PIN, HIGH);
        // Keep resetting the timer until
        // the button is released
        ledTimer = millis();
    }
    else
    {
        // Deactivate the LED when the given duration
        // has passed since the button was released
        if ((millis() - ledTimer) > 5000)
        {
            digitalWrite(LED_PIN, LOW);
        }
    }
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
