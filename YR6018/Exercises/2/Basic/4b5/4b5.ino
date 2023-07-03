/////////////////////////////////////
//////// Custom Data Types //////////
/////////////////////////////////////

enum RgbColorCommand
{
    RED = 1,
    GREEN = 2,
    BLUE = 3
};

enum RgbModeCommand
{
    SOLID = 4,
    SLOW_BLINK = 5,
    FAST_BLINK = 6
};

enum class RgbMode
{
    SOLID,
    SLOW_BLINK,
    FAST_BLINK
};

enum class RgbColor
{
    RED,
    GREEN,
    BLUE
};

/////////////////////////////////////
/////// Button 1 State Code /////////
/////////////////////////////////////

const int BUTTON_1_PIN = 2;

bool button1State = false;

// The time when the button state last changed
unsigned long button1DebounceTimer = 0;
unsigned long button1DebounceTimeout = 50;

// Updates the global variable button1State with
// the current debounced state of the button.
// Do not call this function directly. Use updateButton1 instead.
// https://docs.arduino.cc/built-in-examples/digital/Debounce
void updateButton1State()
{

    if ((millis() - button1DebounceTimer) < button1DebounceTimeout)
    {
        // Exit the function if the timeout has not expired
        return;
    }

    // Because the button is wired as a pulldown circuit,
    // the pin will detect a HIGH signal when the button is pressed,
    // and a LOW signal when the button is not pressed.
    bool newState = digitalRead(BUTTON_1_PIN) == HIGH;

    if (newState != button1State)
    {
        // Reset the debounce timer if the state has changed
        button1DebounceTimer = millis();
    }

    // Update the global variable that
    // the rest of the program uses
    button1State = newState;
}

/////////////////////////////////////
///// Button 1 Edge Detection ///////
/////////////////////////////////////

/*
Becomes true for one loop when the button state has a rising edge.
A rising edge is when the button state changes from 0 to 1
Example:
    if (button1RisingEdge)
    {
        // Do something once per button press
    }
*/
bool button1RisingEdge = false;

// The oposite of button1RisingEdge
bool button1FallingEdge = false;

bool button1PreviousState = false;

// Updates the global variables button1RisingEdge and button1RisingEdge.
// Do not call this function directly. Use updateButton1 instead.
// https://docs.arduino.cc/built-in-examples/digital/StateChangeDetection
void updateButton1EdgeDetection()
{
    // A rising edge is detected when the state is 1 and it used to be 0: 1 && !0
    button1RisingEdge = button1State && !button1PreviousState;

    // A falling edge is detected when the state is 0 and it used to be 1: !0 && 1
    button1FallingEdge = !button1State && button1PreviousState;

    // Update the previous state for the next time this function is called
    button1PreviousState = button1State;
}

/////////////////////////////////////
//////////// Button 1 ///////////////
/////////////////////////////////////

void setupButton1()
{
    pinMode(BUTTON_1_PIN, INPUT);
}

// Updates everything needed to use the global variables
// button1State, button1RisingEdge and button1FallingEdge.
// Call this function in void loop().
void updateButton1()
{
    updateButton1State();
    updateButton1EdgeDetection();
}

/////////////////////////////////////
/////////// Rgb LED Code ////////////
/////////////////////////////////////

const int RGB_RED_PIN = 9;
const int RGB_GREEN_PIN = 6;
const int RGB_BLUE_PIN = 5;

bool rgbState = false;
RgbColor rgbColor = RgbColor::RED;

void updateRgb()
{
    switch (rgbColor)
    {
    case RgbColor::RED:
        digitalWrite(RGB_RED_PIN, rgbState);
        digitalWrite(RGB_GREEN_PIN, LOW);
        digitalWrite(RGB_BLUE_PIN, LOW);
        break;
    case RgbColor::GREEN:
        digitalWrite(RGB_RED_PIN, LOW);
        digitalWrite(RGB_GREEN_PIN, rgbState);
        digitalWrite(RGB_BLUE_PIN, LOW);
        break;
    case RgbColor::BLUE:
        digitalWrite(RGB_RED_PIN, LOW);
        digitalWrite(RGB_GREEN_PIN, LOW);
        digitalWrite(RGB_BLUE_PIN, rgbState);
        break;
    }
}

void setupRgb()
{
    pinMode(RGB_RED_PIN, OUTPUT);
    pinMode(RGB_GREEN_PIN, OUTPUT);
    pinMode(RGB_BLUE_PIN, OUTPUT);
}

/////////////////////////////////////
//////// Rgb Controller Code ////////
/////////////////////////////////////

RgbMode rgbMode = RgbMode::SOLID;

unsigned long rgbBlinkTimer = 0;

void updateRgbMode()
{
    switch (rgbMode)
    {
    case RgbMode::SOLID:
        rgbState = true;
        break;
    case RgbMode::SLOW_BLINK:
        if ((millis() - rgbBlinkTimer) >= 1000)
        {
            rgbState = !rgbState;
            rgbBlinkTimer = millis();
        }
        break;
    case RgbMode::FAST_BLINK:
        if ((millis() - rgbBlinkTimer) >= 250)
        {
            rgbState = !rgbState;
            rgbBlinkTimer = millis();
        }
        break;
    }
}

/////////////////////////////////////
///////// Serial Code ///////////////
/////////////////////////////////////

// The "command" argument of this function is
// the type integer instead of LedCommand
// because the default case allows us to
// handle commands that are not covered by
// the LedCommand enum.
void runSerialCommand(int command)
{
    switch (command)
    {
    ///////////////////////////
    //// Rgb Color Commands ///
    ///////////////////////////
    case RgbColorCommand::RED:
        Serial.println("Witching color to red");
        rgbColor = RgbColor::RED;
        break;
    case RgbColorCommand::GREEN:
        Serial.println("Witching color to green");
        rgbColor = RgbColor::GREEN;
        break;
    case RgbColorCommand::BLUE:
        Serial.println("Witching color to blue");
        rgbColor = RgbColor::BLUE;
        break;
    ///////////////////////////
    //// Rgb Mode Commands ////
    ///////////////////////////
    case RgbModeCommand::SOLID:
        Serial.println("Switching mode to solid");
        rgbMode = RgbMode::SOLID;
        break;
    case RgbModeCommand::SLOW_BLINK:
        Serial.println("Switching mode to slow blink");
        rgbMode = RgbMode::SLOW_BLINK;
        break;
    case RgbModeCommand::FAST_BLINK:
        Serial.println("Switching mode to fast blink");
        rgbMode = RgbMode::FAST_BLINK;
        break;
    ///////////////////////////
    ////// Other Commands /////
    ///////////////////////////
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

void setupSerialCommand()
{
    Serial.println("Please enter a command:");
    Serial.println("Set Color:");
    Serial.println(String(RgbColorCommand::RED) + ": Red");
    Serial.println(String(RgbColorCommand::GREEN) + ": Green");
    Serial.println(String(RgbColorCommand::BLUE) + ": Blue");
    Serial.println("Set Mode:");
    Serial.println(String(RgbModeCommand::SOLID) + ": Solid");
    Serial.println(String(RgbModeCommand::SLOW_BLINK) + ": Slow Blink");
    Serial.println(String(RgbModeCommand::FAST_BLINK) + ": Fast Blink");
}

/////////////////////////////////////
//////// Rgb Sequence Code //////////
/////////////////////////////////////

int rgbSequenceStep = 0;
int rgbSequenceActive = false;
unsigned long rgbSequenceTimer = 0;

void updateRgbSequence()
{
    // Toggle the rgb sequence on or off
    // when button 1 is pressed.
    if (button1RisingEdge)
    {
        rgbSequenceActive = !rgbSequenceActive;
        rgbSequenceStep = 0;
        rgbSequenceTimer = millis();

        if (rgbSequenceActive)
        {
            Serial.println("RGB sequence started");
        }
        else
        {
            Serial.println("RGB sequence stopped");
        }
    }

    if (!rgbSequenceActive)
    {
        // Exit the function
        return;
    }

    switch (rgbSequenceStep)
    {
    case 0:
        // Runs once
        rgbColor = RgbColor::RED;
        rgbMode = RgbMode::SOLID;
        rgbSequenceTimer = millis();
        rgbSequenceStep += 1;
        break;
    case 1:
        // Runs every loop until the timer is up
        if ((millis() - rgbSequenceTimer) >= 10000)
        {
            rgbSequenceStep += 1;
        }
        break;
    case 2:
        // Runs once
        rgbColor = RgbColor::GREEN;
        rgbMode = RgbMode::SLOW_BLINK;
        rgbSequenceTimer = millis();
        rgbSequenceStep += 1;
        break;
    case 3:
        // Runs every loop until the timer is up
        if ((millis() - rgbSequenceTimer) >= 10000)
        {
            rgbSequenceStep += 1;
        }
        break;
    case 4:
        // Runs once
        rgbColor = RgbColor::BLUE;
        rgbMode = RgbMode::FAST_BLINK;
        rgbSequenceTimer = millis();
        rgbSequenceStep += 1;
        break;
    case 5:
        // Runs every loop until the timer is up
        if ((millis() - rgbSequenceTimer) >= 10000)
        {
            rgbSequenceStep = 0;
        }
        break;
    }
}

/////////////////////////////////////
/////////// Main Code ///////////////
/////////////////////////////////////

void setup()
{
    Serial.begin(9600);

    setupSerialCommand();
    setupButton1();
    setupRgb();

    rgbSequenceActive = true;
}

void loop()
{
    updateButton1();
    updateSerial();
    updateRgbSequence();
    updateRgbMode();
    updateRgb();
}
