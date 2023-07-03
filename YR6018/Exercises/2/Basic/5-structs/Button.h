#ifndef BUTTON_h
#define BUTTON_h

struct Button
{
    /////////////////////////////////////
    //////////// State Code /////////////
    /////////////////////////////////////

    int pin = 0;

    bool state = false;

    // The time when the button state last changed
    unsigned long debounceTimer = 0;
    unsigned long debounceTimeout = 50;

    // Updates the global variable State with
    // the current debounced state of the button.
    // Do not call this function directly. Use update instead.
    // https://docs.arduino.cc/built-in-examples/digital/Debounce
    void updateState()
    {

        if ((millis() - debounceTimer) < debounceTimeout)
        {
            // Exit the function if the timeout has not expired
            return;
        }

        // Because the button is wired as a pullup circuit,
        // the pin will detect a LOW signal when the button is pressed,
        // and a HIGH signal when the button is not pressed.
        bool newState = digitalRead(pin) == LOW;

        if (newState != state)
        {
            // Reset the debounce timer if the state has changed
            debounceTimer = millis();
        }

        // Update the global variable that
        // the rest of the program uses
        state = newState;
    }

    /////////////////////////////////////
    ///////// Edge Detection ////////////
    /////////////////////////////////////

    /*
    Becomes true for one loop when the button state has a rising edge.
    A rising edge is when the button state changes from 0 to 1
    Example:
        if (risingEdge)
        {
            // Do something once per button press
        }
    */
    bool risingEdge = false;

    // The oposite of risingEdge
    bool fallingEdge = false;

    bool previousState = false;

    // Updates the global variables risingEdge and risingEdge.
    // Do not call this function directly. Use update instead.
    // https://docs.arduino.cc/built-in-examples/digital/StateChangeDetection
    void updateEdgeDetection()
    {
        // A rising edge is detected when the state is 1 and it used to be 0: 1 && !0
        risingEdge = state && !previousState;

        // A falling edge is detected when the state is 0 and it used to be 1: !0 && 1
        fallingEdge = !state && previousState;

        // Update the previous state for the next time this function is called
        previousState = state;
    }

    /////////////////////////////////////
    //////////// ///////////////
    /////////////////////////////////////

    void setup()
    {
        pinMode(pin, INPUT_PULLUP);
    }

    // Updates everything needed to use the global variables
    // State, risingEdge and fallingEdge.
    // Call this function in void loop().
    void update()
    {
        updateState();
        updateEdgeDetection();
    }
};

#endif
