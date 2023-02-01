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

Timer timer1;

int button1Pin = 2;

void setup()
{
    Serial.begin(9600);

    pinMode(button1Pin, INPUT);
}

void loop()
{
    if (!timer1.isFinished(3000))
    {
        Serial.println("Timer1");
    }
    
    if (digitalRead(button1Pin))
    {
        timer1.reset();
    }

    delay(100);
}
