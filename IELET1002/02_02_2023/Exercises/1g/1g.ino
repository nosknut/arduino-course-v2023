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
Timer timer3;
Timer timer4;
Timer timer5;

// Set pin values
int led1Pin = 3;
int led2Pin = 5;
int led3Pin = 6;
int led4Pin = 9;
int led5Pin = 10;

int button1Pin = 2;

void setup()
{
    Serial.begin(9600);

    pinMode(button1Pin, INPUT);

    pinMode(led1Pin, OUTPUT);
    pinMode(led2Pin, OUTPUT);
    pinMode(led3Pin, OUTPUT);
    pinMode(led4Pin, OUTPUT);
    pinMode(led5Pin, OUTPUT);
}

void loop()
{
    if (!digitalRead(button1Pin))
    {
        timer1.reset();
        timer2.reset();
        timer3.reset();
        timer4.reset();
        timer5.reset();

        digitalWrite(led1Pin, LOW);
        digitalWrite(led2Pin, LOW);
        digitalWrite(led3Pin, LOW);
        digitalWrite(led4Pin, LOW);
        digitalWrite(led5Pin, LOW);
    }
    else
    {
        if (timer1.isFinished(1000))
        {
            digitalWrite(led1Pin, HIGH);
        }

        if (timer2.isFinished(2000))
        {
            digitalWrite(led2Pin, HIGH);
        }

        if (timer3.isFinished(3000))
        {
            digitalWrite(led3Pin, HIGH);
        }

        if (timer4.isFinished(4000))
        {
            digitalWrite(led4Pin, HIGH);
        }

        if (timer5.isFinished(5000))
        {
            digitalWrite(led5Pin, HIGH);
        }
    }

    delay(100);
}
