struct Potmeter {
    int pin;
    int value = 0;
    int convertedValue = 0;

    void update() {
        value = analogRead(pin);
        convertedValue = map(value, 0, 1023, 0, 9);
    }
};
