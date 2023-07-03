// A recipe for non-blocking arduino code can be found here:
// https://github.com/nosknut/arduino-course-v2023/blob/main/IELET1002/ArduinoExampleCode/FromBlockingToNonBlocking/FromBlockingToNonBlocking.ino

unsigned long timer = 0;

int step = 0;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    switch (step)
    {
    case 0:
        // Print once and move to the next step
        Serial.println("step 0");
        step += 1;
        // Reset the timer
        timer = millis();
        break;
    case 1:
        // Do nothing until the time has passed
        if ((millis() - timer) >= 1000)
        {
            // Move to the next step
            step += 1;
        }
        break;
    case 2:
        // Print once and move to the next step
        Serial.println("step 1");
        step += 1;
        // Reset the timer
        timer = millis();
        break;
    case 3:
        // Do nothing until the time has passed
        if ((millis() - timer) >= 1000)
        {
            // Move to the first step
            step = 0;
        }
        break;
    }
}
