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
Timer timer2;

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
        if (timer2.isFinished(500))
        {
            Serial.println("Timer1");
            timer2.reset();
        }
    }
    
    if (digitalRead(button1Pin))
    {
        timer1.reset();
    }

    delay(100);
}
