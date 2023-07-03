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

void runSerialCommand(int command)
{
    switch (command)
    {
    case 1:
        Serial.println("Turning LED on");
        digitalWrite(LED_PIN, HIGH);
        break;
    case 2:
        Serial.println("Turning LED off");
        digitalWrite(LED_PIN, LOW);
        break;
    case 3:
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

// No longer in use.
// For some reason the Serial monitor
// outputs the command 0 as well as the
// command you enter. This function
// was created to investigate.
void updateDebugUnwantedCommand()
{
    if (Serial.available() > 0)
    {
        int command = Serial.read();
        Serial.println(command);

        // When reading the command as a char,
        // the extra command is not printed,
        // however as int, we can see that we receive
        // a character with the ascii value of 10.
        // According to this ascii table:
        // https://www.asciitable.com/
        // We can see that 10 is the ascii value for a newline
        // character LF (\n).
        // This explains why it was invisible when reading
        // it as a char, since newline characters are invisible.
    }
}

void setupSerialCommand()
{
    Serial.println("Please enter a command:");
    Serial.println("1: Turn LED on");
    Serial.println("2: Turn LED off");
    Serial.println("3: Flash LED");
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
