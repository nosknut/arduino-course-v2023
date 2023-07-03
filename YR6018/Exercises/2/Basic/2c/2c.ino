// Simulation:
// https://wokwi.com/projects/368604124891332609

// A recipe for non-blocking arduino code can be found here:
// https://github.com/nosknut/arduino-course-v2023/blob/main/IELET1002/ArduinoExampleCode/FromBlockingToNonBlocking/FromBlockingToNonBlocking.ino

/////////////////////////////////////
///////////// LED Code //////////////
/////////////////////////////////////

const int LED_1_PIN = 6;
const int LED_2_PIN = 10;

void setupLeds()
{
    pinMode(LED_1_PIN, OUTPUT);
    pinMode(LED_2_PIN, OUTPUT);
}

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

    // Because the button is wired as a pullup circuit,
    // the pin will detect a LOW signal when the button is pressed,
    // and a HIGH signal when the button is not pressed.
    bool newState = digitalRead(BUTTON_1_PIN) == LOW;

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
    pinMode(BUTTON_1_PIN, INPUT_PULLUP);
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
/////// Button 2 State Code /////////
/////////////////////////////////////

const int BUTTON_2_PIN = 12;

bool button2State = false;

// The time when the button state last changed
unsigned long button2DebounceTimer = 0;
unsigned long button2DebounceTimeout = 50;

// Updates the global variable button2State with
// the current debounced state of the button.
// Do not call this function directly. Use updateButton2 instead.
// https://docs.arduino.cc/built-in-examples/digital/Debounce
void updateButton2State()
{
    
    if ((millis() - button2DebounceTimer) < button2DebounceTimeout)
    {
        // Exit the function if the timeout has not expired
        return;
    }

    // Because the button is wired as a pullup circuit,
    // the pin will detect a LOW signal when the button is pressed,
    // and a HIGH signal when the button is not pressed.
    bool newState = digitalRead(BUTTON_2_PIN) == LOW;

    if (newState != button2State)
    {
        // Reset the debounce timer if the state has changed
        button2DebounceTimer = millis();
    }

    // Update the global variable that
    // the rest of the program uses
    button2State = newState;
}

/////////////////////////////////////
///// Button 2 Edge Detection ///////
/////////////////////////////////////

/*
Becomes true for one loop when the button state has a rising edge.
A rising edge is when the button state changes from 0 to 1
Example:
    if (button2RisingEdge)
    {
        // Do something once per button press
    }
*/
bool button2RisingEdge = false;

// The oposite of button2RisingEdge
bool button2FallingEdge = false;

bool button2PreviousState = false;

// Updates the global variables button2RisingEdge and button2RisingEdge.
// Do not call this function directly. Use updateButton2 instead.
// https://docs.arduino.cc/built-in-examples/digital/StateChangeDetection
void updateButton2EdgeDetection()
{
    // A rising edge is detected when the state is 2 and it used to be 0: 2 && !0
    button2RisingEdge = button2State && !button2PreviousState;

    // A falling edge is detected when the state is 0 and it used to be 1: !0 && 1
    button2FallingEdge = !button2State && button2PreviousState;

    // Update the previous state for the next time this function is called
    button2PreviousState = button2State;
}

/////////////////////////////////////
//////////// Button 2 ///////////////
/////////////////////////////////////

void setupButton2()
{
    pinMode(BUTTON_2_PIN, INPUT_PULLUP);
}

// Updates everything needed to use the global variables
// button2State, button2RisingEdge and button2FallingEdge.
// Call this function in void loop().
void updateButton2()
{
    updateButton2State();
    updateButton2EdgeDetection();
}

/////////////////////////////////////
/////// Square Modulator Code ///////
/////////////////////////////////////

/**
 * @param frequency The frequency of the square wave in Hz (Hertz).
 * @param amplitude The maximum value of the wave.
 * @param offset The offset of the square wave in ms (milliseconds).
 * @return A square wave with the given frequency and offset.
*/
long getSquareWave(double frequency, long amplitude, long offset)
{
    // The period of the square wave in ms (milliseconds)
    // the period is the time in ms (milliseconds) it takes for the
    // wave to complete one cycle (from 0 to 1 and back to 0)
    unsigned long period = 1000 / frequency;

    // The time since the program started + the offset
    // that skews the function in time.
    unsigned long time = millis() + offset;

    // https://www.arduino.cc/reference/en/language/structure/arithmetic-operators/remainder/
    // https://infosys.beckhoff.com/content/1033/tf5100_tc3_nc_i/Images/gif/3292507403__en-US__Web.gif
    double periodicTime = time % period;

    unsigned long halfPeriod = period / 2;

    // The square wave is 1 for the first half of the
    // period, and 0 for the second half of the period
    bool currentSquareWaveValue = periodicTime < halfPeriod;

    // Generate a value that is either equal to 0 or the amplitude
    if (currentSquareWaveValue == 1)
    {
        return amplitude;
    }
    else
    {
        return 0;
    }
}

/////////////////////////////////////
////// Triangle Modulator Code //////
/////////////////////////////////////

/**
 * Generates the following type of signal:
 * https://www.statisticshowto.com/wp-content/uploads/2019/12/triangle-wave-function.png
 * @param frequency The frequency of the wave in Hz (Hertz).
 * @param amplitude The maximum value of the wave.
 * @param offset The offset of the wave in ms (milliseconds).
 * @return A triangle wave with the given frequency and maximum value.
*/
unsigned long getTriangleWave(double frequency, long amplitude, long offset)
{
    // The period of the square wave in ms (milliseconds)
    // the period is the time in ms (milliseconds) it takes for the
    // wave to complete one cycle (from 0 to max and back to 0)
    unsigned long period = 1000 / frequency;

    // The time since the program started + the offset
    // that skews the function in time.
    unsigned long time = millis() + offset;
    
    // https://www.arduino.cc/reference/en/language/structure/arithmetic-operators/remainder/
    // https://infosys.beckhoff.com/content/1033/tf5100_tc3_nc_i/Images/gif/3292507403__en-US__Web.gif
    unsigned long periodicTime = time % period;

    unsigned long halfPeriod = period / 2;

    // The signal is 0 at the start of the period,
    // and increases linearly until half way through the period
    // where it is equal to the amplitude.
    // Then it decreases linearly until the end of the period
    // where it is equal to 0 again.
    if (periodicTime < halfPeriod)
    {
        // https://www.arduino.cc/reference/en/language/functions/math/map/
        return map(periodicTime, 0, halfPeriod, 0, amplitude);
    }
    else
    {
        // https://www.arduino.cc/reference/en/language/functions/math/map/
        return map(periodicTime, halfPeriod, period, amplitude, 0);
    }
}

/////////////////////////////////////
///// Update Winner Led Code ////////
/////////////////////////////////////

int winnerLedPin = 0;
bool runCelebrateWinnerLed = false;

void startWinnerCelebrationLed(int ledPin)
{
    winnerLedPin = ledPin;
    runCelebrateWinnerLed = true;
}

void stopWinnerCelebrationLed()
{
    runCelebrateWinnerLed = false;
    digitalWrite(winnerLedPin, LOW);
}

// Rapidly fades the winner LED in and out
void updateWinnerCelebrationLed()
{
    if (!runCelebrateWinnerLed)
    {
        // Exit the function
        return;
    }

    analogWrite(winnerLedPin, getTriangleWave(1, 255, 0));
}

/////////////////////////////////////
///// Update Looser Led Code ////////
/////////////////////////////////////

int looserLedPin = 0;
bool runHumiliateLooserLed = false;

void startLooserHumiliationLed(int ledPin)
{
    looserLedPin = ledPin;
    runHumiliateLooserLed = true;
}

void stopLooserHumiliationLed()
{
    runHumiliateLooserLed = false;
    digitalWrite(looserLedPin, LOW);
}

// Rapidly fades the winner LED in and out
void updateLooserHumiliationLed()
{
    if (!runHumiliateLooserLed)
    {
        // Exit the function
        return;
    }

    analogWrite(looserLedPin, getSquareWave(4, 255, 0));
}

/////////////////////////////////////
////////// Buzzer Code //////////////
/////////////////////////////////////

const int BUZZER_PIN = 11;

bool playWinnerCelebrationBuzzer = false;
bool playLooserHumiliationBuzzer = false;

void stopBuzzer()
{
    playWinnerCelebrationBuzzer = false;
    playLooserHumiliationBuzzer = false;
    noTone(BUZZER_PIN);
}

void startWinnerCelebrationBuzz()
{
    stopBuzzer();
    playWinnerCelebrationBuzzer = true;
}

void startLooserHumiliationBuzz()
{
    stopBuzzer();
    playLooserHumiliationBuzzer = true;
}

// Plays the winner celebration buzz
// when enabled.
// Do not call this function directly.
// Use updateBuzzer() instead.
void updateWinnerCelebrationBuzz()
{
    if (playWinnerCelebrationBuzzer)
    {
        // The code below uses the same logic as the LED code.
        // The frequency of the tone
        // changes from 500 Hz to (500 + 1500 = 2000) Hz and back to 500 Hz
        // 2 times per second in a triangular pattern.
        // https://www.statisticshowto.com/wp-content/uploads/2019/12/triangle-wave-function.png
        tone(BUZZER_PIN, getTriangleWave(2, 1500, 0) + 500);
    }
}

// Plays the looser humiliation buzz
// when enabled.
// Do not call this function directly.
// Use updateBuzzer() instead.
void updateLooserHumiliationBuzz()
{
    if (playLooserHumiliationBuzzer)
    {
        // The code below uses the same logic as the LED code.
        // The frequency of the tone
        // switches between 100 Hz and (100 + 100 = 200) Hz
        // 4 times per second in a square pattern.
        // https://www.statisticshowto.com/wp-content/uploads/2019/12/square-wave-function.png
        tone(BUZZER_PIN, getSquareWave(4, 100, 0) + 100);
    }
}

// Call this function in void loop()
// to update the tones for the buzzer.
void updateBuzzer()
{
    updateWinnerCelebrationBuzz();
    updateLooserHumiliationBuzz();
}

void setupBuzzer()
{
    pinMode(BUZZER_PIN, OUTPUT);
}

/////////////////////////////////////
////// Winner Celebration Code //////
/////////////////////////////////////

bool celebrateWinner = false;
unsigned long winnerCelebrationTimer = 0;

void startWinnerCelebration(int winnerLedPin)
{
    // Start the winner celebration
    celebrateWinner = true;
    winnerCelebrationTimer = millis();
    startWinnerCelebrationLed(winnerLedPin);
    startWinnerCelebrationBuzz();
}

// Stops the winner celebration after 3 seconds
// Call this function in void loop()
void updateWinnerCelebration()
{
    if (!celebrateWinner)
    {
        // Exit the function
        return;
    }

    if ((millis() - winnerCelebrationTimer) < 3000)
    {
        // Exit the function until 3 seconds have passed
        return;
    }

    // Stop the winner celebration
    celebrateWinner = false;
    stopWinnerCelebrationLed();
    stopBuzzer();
}

/////////////////////////////////////
////// Looser Humiliation Code //////
/////////////////////////////////////

bool humiliateLooser = false;
unsigned long looserHumiliationTimer = 0;

void startLooserHumiliation(int looserLedPin)
{
    // Start the looser humiliation
    humiliateLooser = true;
    looserHumiliationTimer = millis();
    startLooserHumiliationLed(looserLedPin);
    startLooserHumiliationBuzz();
}

// Stops the looser humiliation after 4 seconds
// Call this function in void loop()
void updateLooserHumiliation()
{
    if (!humiliateLooser)
    {
        // Exit the function
        return;
    }

    if ((millis() - looserHumiliationTimer) < 4000)
    {
        // Exit the function until 4 seconds have passed
        return;
    }

    // Stop the looser humiliation
    humiliateLooser = false;
    stopLooserHumiliationLed();
    stopBuzzer();
}

/////////////////////////////////////
/// Winner And Looser Test Code /////
/////////////////////////////////////

void updateWinnerAndLooserTest()
{
    if (button1RisingEdge)
    {
        startWinnerCelebration(LED_1_PIN);
    }

    if (button2RisingEdge)
    {
        startLooserHumiliation(LED_2_PIN);
    }
}

/////////////////////////////////////
/////////// Main Code ///////////////
/////////////////////////////////////

void setup()
{
    Serial.begin(9600);

    setupLeds();
    setupButton1();
    setupButton2();
    setupBuzzer();
}

void loop()
{
    updateButton1();
    updateButton2();

    updateWinnerAndLooserTest();

    updateWinnerCelebration();
    updateLooserHumiliation();

    updateWinnerCelebrationLed();
    updateLooserHumiliationLed();
    updateBuzzer();
}
