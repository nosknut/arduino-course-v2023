// TinkerCad: https://www.tinkercad.com/things/dGvpYIUV9cP

int step = 0;

int buttonPin = 2;
unsigned long timer = 0;

void blocking()
{
    Serial.println("A");

    // Prints B many times while waiting for button to be pressed
    while (!digitalRead(buttonPin))
    {
        Serial.println("B");
    }

    Serial.println("C");

    delay(1000);

    Serial.println("D");

    // Resets timer
    timer = millis();

    // Prints E many times for one second
    while ((millis() - timer) < 1000)
    {
        Serial.println("E");
    }

    Serial.println("F");

    // For-loop start; condition&exit; increment
    for (int i = 0; i <= 5; i++)
    {
        // For-loop content
        Serial.println(i);
    }

    Serial.println("G");
}

int i = 0;

void nonBlocking()
{
    switch (step)
    {
    case 0:
        Serial.println("A");
        step += 1;
        break;
    case 1:
        // Prints B many times while waiting for button to be pressed
        Serial.println("B");
        if (digitalRead(buttonPin))
        {
            step += 1;
        }
        break;
    case 2:
        Serial.println("C");
        timer = millis();
        step += 1;
    case 3:
        if ((millis() - timer) >= 1000)
        {
            step += 1;
        }
        break;
    case 4:
        Serial.println("D");
        // Resets timer
        timer = millis();
        step += 1;
        break;
    case 5:
        // Prints E many times for one second
        Serial.println("E");
        if ((millis() - timer) >= 1000)
        {
            step += 1;
        }
        break;
    case 6:
        Serial.println("F");
        // For-loop start
        i = 0;
        step += 1;
        break;
    case 7:
        // For-loop condition
        if (i <= 5)
        {
            // For-loop content
            Serial.println(i);
            
            // For-loop increment
            i++;
        }
        else
        {
            // For-loop exit
            step += 1;
        }
        break;
    case 8:
        Serial.println("G");
        step += 1;
        break;
    default:
        step = 0;
        break;
    }
}

void setup()
{
    Serial.begin(9600);

    pinMode(buttonPin, INPUT);
}

void loop()
{
    nonBlocking();

    // 100ms delay to prevent TinkerCad from slowing down
    // on an actual arduino the line below should be deleted
    delay(100);
}
