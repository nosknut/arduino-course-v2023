/////////////////////////////////////
///////// Random LED Code ///////////
/////////////////////////////////////

const int RANDOM_LED_PIN = 5;

bool randomLedState = false;
unsigned long randomLedDuration = 0;
unsigned long randomLedTimer = 0;

void updateRandomLed()
{
    if ((millis() - randomLedTimer) >= randomLedDuration)
    {
        // Toggle the LED on or off
        randomLedState = !randomLedState;
        digitalWrite(RANDOM_LED_PIN, randomLedState);

        randomLedDuration = random(3000, 6000);
        Serial.print("New random duration: ");
        Serial.println(randomLedDuration);
        randomLedTimer = millis();
    }
}

void setupRandomLed()
{
    pinMode(RANDOM_LED_PIN, OUTPUT);
}

/////////////////////////////////////
/////////// Main Code ///////////////
/////////////////////////////////////

void setup()
{
    Serial.begin(9600);
    randomSeed(analogRead(0));

    setupRandomLed();
}

void loop()
{
    updateRandomLed();
}
