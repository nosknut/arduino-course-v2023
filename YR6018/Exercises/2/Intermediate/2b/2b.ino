// A recipe for non-blocking arduino code can be found here:
// https://github.com/nosknut/arduino-course-v2023/blob/main/IELET1002/ArduinoExampleCode/FromBlockingToNonBlocking/FromBlockingToNonBlocking.ino

/////////////////////////////////////
///////// Custom Data Types /////////
/////////////////////////////////////

// NB! Custom data types in C++ (Arduino) must be defined before any
// functions, or the compiler will give an error saying the data type is not defined.
// Normally, all variables and data types would be defined at the top of the file,
// but an exception is made for this example to make it easier to follow.

// Is used to make setting the state of the LED indicator more readable.
// Using enum class instead of enum because it is safer and groups the enum
// values under the enum name.
// In practice the difference between enum class and enum is the way it is used:
// LedIndicatorMode::CASE_1 instead of just CASE_1.
// If you are interested in the reasons why you should use one over the other, see the post below:
// https://www.geeksforgeeks.org/enum-classes-in-c-and-their-advantage-over-enum-datatype/
enum class LedIndicatorMode
{
    OFF,
    CASE_1,
    CASE_2,
    CASE_3,
    CASE_4
};

/////////////////////////////////////
//////////// Button A State /////////
/////////////////////////////////////

const int BUTTON_A_PIN = 3;

// The debounced state of the button
bool buttonAState = false;

// The time when the button state last changed
unsigned long debounceTimer = 0;
unsigned long debounceTimeout = 50;

// Updates the global variable buttonAState with
// the current debounced state of the button.
// Do not call this function directly. Use updateButtonA instead.
// https://docs.arduino.cc/built-in-examples/digital/Debounce
void updateButtonAState()
{
    if ((millis() - debounceTimer) < debounceTimeout)
    {
        // Exit the function if the timeout has not expired
        return;
    }

    bool newState = digitalRead(BUTTON_A_PIN);

    if (newState != buttonAState)
    {
        // Reset the debounce timer if the state has changed
        debounceTimer = millis();
    }

    // Update the global variable that
    // the rest of the program uses
    buttonAState = newState;
}

/////////////////////////////////////
///// Button A Edge Detection ///////
/////////////////////////////////////

/*
Becomes true for one loop when the button state has a rising edge.
A rising edge is when the button state changes from 0 to 1
Example:
    if (buttonARisingEdge)
    {
        // Do something once per button press
    }
*/
bool buttonARisingEdge = false;

// The oposite of buttonARisingEdge
bool buttonAFallingEdge = false;

bool buttonAPreviousState = false;

// Updates the global variables buttonARisingEdge and buttonARisingEdge.
// Do not call this function directly. Use updateButtonA instead.
// https://docs.arduino.cc/built-in-examples/digital/StateChangeDetection
void updateButtonAEdgeDetection()
{
    // A rising edge is detected when the state is 1 and it used to be 0: 1 && !0
    buttonARisingEdge = buttonAState && !buttonAPreviousState;

    // A falling edge is detected when the state is 0 and it used to be 1: !0 && 1
    buttonAFallingEdge = !buttonAState && buttonAPreviousState;

    // Update the previous state for the next time this function is called
    buttonAPreviousState = buttonAState;
}

/////////////////////////////////////
//////////// Button A ///////////////
/////////////////////////////////////

void setupButtonA()
{
    pinMode(BUTTON_A_PIN, INPUT);
}

// Updates everything needed to use the global variables
// buttonAState, buttonARisingEdge and buttonAFallingEdge.
// Call this function in void loop().
void updateButtonA()
{
    updateButtonAState();
    updateButtonAEdgeDetection();
}

/////////////////////////////////////
//////////// LED Code ///////////////
/////////////////////////////////////

const int CASE_1_LED_PIN = 6;
const int CASE_2_LED_PIN = 7;
const int CASE_3_LED_PIN = 8;
const int CASE_4_LED_PIN = 9;

// Activates and deactivates the LEDS in the case LED
// indicator based on the provided indicator mode.
// Why use an enum for this instead of an int with the numbers 0 to 4?
// Because it makes the code more readable.
int setCaseLedIndicatorMode(LedIndicatorMode mode)
{
    switch (mode)
    {
    case LedIndicatorMode::OFF:
        digitalWrite(CASE_1_LED_PIN, LOW);
        digitalWrite(CASE_2_LED_PIN, LOW);
        digitalWrite(CASE_3_LED_PIN, LOW);
        digitalWrite(CASE_4_LED_PIN, LOW);
        break;
    case LedIndicatorMode::CASE_1:
        digitalWrite(CASE_1_LED_PIN, HIGH);
        digitalWrite(CASE_2_LED_PIN, LOW);
        digitalWrite(CASE_3_LED_PIN, LOW);
        digitalWrite(CASE_4_LED_PIN, LOW);
        break;
    case LedIndicatorMode::CASE_2:
        digitalWrite(CASE_1_LED_PIN, LOW);
        digitalWrite(CASE_2_LED_PIN, HIGH);
        digitalWrite(CASE_3_LED_PIN, LOW);
        digitalWrite(CASE_4_LED_PIN, LOW);
        break;
    case LedIndicatorMode::CASE_3:
        digitalWrite(CASE_1_LED_PIN, LOW);
        digitalWrite(CASE_2_LED_PIN, LOW);
        digitalWrite(CASE_3_LED_PIN, HIGH);
        digitalWrite(CASE_4_LED_PIN, LOW);
        break;
    case LedIndicatorMode::CASE_4:
        digitalWrite(CASE_1_LED_PIN, LOW);
        digitalWrite(CASE_2_LED_PIN, LOW);
        digitalWrite(CASE_3_LED_PIN, LOW);
        digitalWrite(CASE_4_LED_PIN, HIGH);
        break;
    }
    return 1;
}

void setupCaseLedIndicator()
{
    pinMode(CASE_1_LED_PIN, OUTPUT);
    pinMode(CASE_2_LED_PIN, OUTPUT);
    pinMode(CASE_3_LED_PIN, OUTPUT);
    pinMode(CASE_4_LED_PIN, OUTPUT);
}

/////////////////////////////////////
/////////// Buzzer Code /////////////
/////////////////////////////////////

const int BUZZER_PIN = 10;

void playCaseChangeBuzz()
{
    tone(BUZZER_PIN, 500, 10);
}

void setupBuzzer()
{
    pinMode(BUZZER_PIN, OUTPUT);
}

/////////////////////////////////////
//////////// Main Code //////////////
/////////////////////////////////////

int step = 0;

void setup()
{
    Serial.begin(9600);
    setupButtonA();
    setupCaseLedIndicator();
}

void updateSequence()
{
    switch (step)
    {
    //////////////////
    ///// Case 1 /////
    //////////////////
    case 0:
        // Code that should run once during case 1
        setCaseLedIndicatorMode(LedIndicatorMode::CASE_1);
        playCaseChangeBuzz();

        Serial.println("Case 1 is active");
        step += 1;
        break;
    case 1:
        // Code that should run over and over again during case 1
        if (buttonARisingEdge)
        {
            step += 1;
        }
        break;
    //////////////////
    ///// Case 2 /////
    //////////////////
    case 2:
        // Code that should run once during case 2
        setCaseLedIndicatorMode(LedIndicatorMode::CASE_2);
        playCaseChangeBuzz();

        Serial.println("Case 2 is active");
        step += 1;
        break;
    case 3:
        // Code that should run over and over again during case 2
        if (buttonARisingEdge)
        {
            step += 1;
        }
        break;
    //////////////////
    ///// Case 3 /////
    //////////////////
    case 4:
        // Code that should run once during case 3
        setCaseLedIndicatorMode(LedIndicatorMode::CASE_3);
        playCaseChangeBuzz();

        Serial.println("Case 3 is active");
        step += 1;
        break;
    case 5:
        // Code that should run over and over again during case 3
        if (buttonARisingEdge)
        {
            step += 1;
        }
        break;
    //////////////////
    ///// Case 4 /////
    //////////////////
    case 6:
        // Code that should run once during case 4
        setCaseLedIndicatorMode(LedIndicatorMode::CASE_4);
        playCaseChangeBuzz();

        Serial.println("Case 4 is active");
        step += 1;
        break;
    case 7:
        // Code that should run over and over again during case 4
        if (buttonARisingEdge)
        {
            step = 0;
        }
        break;
    }
}

void loop()
{
    updateButtonA();
    updateSequence();
}
