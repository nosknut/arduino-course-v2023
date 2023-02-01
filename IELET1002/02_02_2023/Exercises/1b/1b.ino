int button1Pin = 2;

void setup()
{
    Serial.begin(9600);

    pinMode(button1Pin, INPUT);
}

void loop()
{
    delay(3000);
    Serial.println("Timer1, Elapsed Time: 3000");

    if (digitalRead(button1Pin))
    {
        Serial.println("Button 1 pressed");
    }
}
