// A recipe for non-blocking arduino code can be found here:
// https://github.com/nosknut/arduino-course-v2023/blob/main/IELET1002/ArduinoExampleCode/FromBlockingToNonBlocking/FromBlockingToNonBlocking.ino

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
//////////// Main Code //////////////
/////////////////////////////////////

int step = 0;

void setup()
{
    Serial.begin(9600);
    setupButtonA();
}

void updateSequence()
{
    switch (step)
    {
    case 0:
        // Print once and move to the next step
        Serial.println("Step 0");
        step += 1;
        break;
    case 1:
        // Do nothing until button is pressed
        if (buttonARisingEdge)
        {
            step += 1;
        }
        break;
    case 2:
        // Print once and move to the next step
        Serial.println("Step 1");
        step += 1;
        break;
    case 3:
        // Do nothing until button is pressed
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
