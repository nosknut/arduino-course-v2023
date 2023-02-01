struct Potmeter {
    int pin;
    int value = 0;

    void update() {
        value = analogRead(pin);
    }
};

Potmeter potmeter1;

void setup()
{
    Serial.begin(9600);
    
    potmeter1.pin = A0;
    pinMode(potmeter1.pin, INPUT);
}

void loop()
{
    potmeter1.update();

    Serial.println(potmeter1.value);

    delay(100);
}
