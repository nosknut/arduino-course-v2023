/////////////////////////////////////
//////// Custom Data Types //////////
/////////////////////////////////////

enum LedCommand
{
    TURN_ON = 1,
    TURN_OFF = 2,
    BLINK = 3
};

/////////////////////////////////////
///////////// LED Code //////////////
/////////////////////////////////////

const int LED_PIN = 5;

void setupLed()
{
    pinMode(LED_PIN, OUTPUT);
}

/////////////////////////////////////
/////////// LED Blink Code //////////
/////////////////////////////////////

bool ledBlinkActive = false;

bool ledState = false;

unsigned long ledBlinkTimer = 0;
unsigned long ledBlinkDurationTimer = 0;

void activateLedBlink()
{
    ledState = true;
    ledBlinkActive = true;
    ledBlinkTimer = millis();
    ledBlinkDurationTimer = millis();
}

void updateLedBlink()
{
    if (!ledBlinkActive)
    {
        // Exit the function
        return;
    }

    if ((millis() - ledBlinkDurationTimer) >= 5000)
    {
        ledBlinkActive = false;
        digitalWrite(LED_PIN, LOW);
        // Exit the function
        return;
    }

    if ((millis() - ledBlinkTimer) >= 500)
    {
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
        ledBlinkTimer = millis();
    }
}

/////////////////////////////////////
///////// Serial Code ///////////////
/////////////////////////////////////

// The "command" argument of this function is
// the type integer instead of LedCommand
// because the default case allows us to
// handle commands that are not covered by
// the LedCommand enum.
void runSerialCommand(int command)
{
    switch (command)
    {
    case LedCommand::TURN_ON:
        Serial.println("Turning LED on");
        digitalWrite(LED_PIN, HIGH);
        break;
    case LedCommand::TURN_OFF:
        Serial.println("Turning LED off");
        digitalWrite(LED_PIN, LOW);
        break;
    case LedCommand::BLINK:
        Serial.println("Blinking LED");
        activateLedBlink();
        break;
    default:
        Serial.println("Invalid command: " + String(command));
        break;
    }
}

void updateSerial()
{
    if (Serial.available() > 0)
    {
        char command = Serial.parseInt();

        if (command == 0)
        {
            // Do nothing
            // This is executed when the Serial Monitor
            // receives a newline (\n) character at
            // the end of every message.
            return;
        }

        runSerialCommand(command);
    }
}

void setupSerialCommand()
{
    Serial.println("Please enter a command:");
    Serial.println(String(LedCommand::TURN_ON) + ": Turn LED on");
    Serial.println(String(LedCommand::TURN_OFF) + ": Turn LED off");
    Serial.println(String(LedCommand::BLINK) + ": Flash LED");
}

/////////////////////////////////////
/////////// Main Code ///////////////
/////////////////////////////////////

void setup()
{
    Serial.begin(9600);

    setupSerialCommand();
    setupLed();
}

void loop()
{
    updateSerial();
    updateLedBlink();
}
