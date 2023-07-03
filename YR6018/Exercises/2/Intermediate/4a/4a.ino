#include <Servo.h>
#include "CatDX4001.h"

// https://wokwi.com/projects/368049679345794049

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

enum class HatchStatus
{
    OPENING,
    CLOSING,
    OPEN,
    CLOSED
};

enum class RgbMode
{
    OFF,
    PULSE
};

/////////////////////////////////////
//////////// Button A State /////////
/////////////////////////////////////

const int BUTTON_A_PIN = 2;

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

const int CASE_1_LED_PIN = 4;
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

unsigned long int case2BuzzCounterTimer = 0;
int case2RemainingBuzzCounter = 0;

void playCaseChangeBuzz()
{
    tone(BUZZER_PIN, 500, 10);
}

void playHatchStepBuzz()
{
    tone(BUZZER_PIN, 1000, 100);
}

void playHatchClosedBuzz()
{
    tone(BUZZER_PIN, 250, 1000);
}

void playCase2ShortBuzz()
{
    tone(BUZZER_PIN, 500, 100);
}

void playCase2ShortBuzzes(int buzzCount)
{
    case2RemainingBuzzCounter = buzzCount;
}

void updateCase2ShortBuzz()
{
    if (case2RemainingBuzzCounter > 0)
    {
        if ((millis() - case2BuzzCounterTimer) >= 500)
        {
            playCase2ShortBuzz();
            case2RemainingBuzzCounter--;
            case2BuzzCounterTimer = millis();
        }
    }
}

void playCase2ConstantBuzz()
{
    tone(BUZZER_PIN, 250);
}

void stopBuzzer()
{
    noTone(BUZZER_PIN);
}

void setupBuzzer()
{
    pinMode(BUZZER_PIN, OUTPUT);
}

/////////////////////////////////////
/// Hatch Servo Controller Code /////
/////////////////////////////////////

// The code in this section slowly moves the
// servo to a specific position. It does not
// contain any logic relating to what the target
// position should be.

int hatchServoPosition = 0;

// Use this variable to set the desired servo position
int hatchServoTargetPosition = 0;

const int HATCH_SERVO_PIN = 11;

Servo hatchServo;

unsigned long hatchServoTimer = 0;
unsigned long hatchServoSpeed = 20;

// Moves the hatch servo to hatchServoTargetPosition at a specific speed.
// Call this function in void loop()
void updateHatchServo()
{
    if ((millis() - hatchServoTimer) < hatchServoSpeed)
    {
        // Exit the funtion until the timer finished
        return;
    }

    if (hatchServoPosition > hatchServoTargetPosition)
    {
        hatchServoPosition -= 1;
    }

    if (hatchServoPosition < hatchServoTargetPosition)
    {
        hatchServoPosition += 1;
    }

    hatchServo.write(hatchServoPosition);
    hatchServoTimer = millis();
}

// Call this function in void setup()
void setupHatchServo()
{
    hatchServo.attach(HATCH_SERVO_PIN);
}

/////////////////////////////////////
////// Hatch Sequence Code //////////
/////////////////////////////////////

// This code contains the hatch opening and closing sequence.
// The hatch code is split into a servo section and a sequence
// secion because smoothly moving the servo and controlling the
// hatch steps are different tasks that become much more complex
// when combined. Now that these responsibilities are divided,
// we can simply use the hatchServoTargetPosition variable to
// request a specific servo position, and we can trust that
// the servo section will take care of the details.

// Indicates the status of the hatch sequence.
// Do not change this variable directly.
// Use openHatch() and closeHatch() instead.
HatchStatus hatchStatus = HatchStatus::CLOSED;

unsigned long hatchTimer = 0;
unsigned long hatchStepDuration = 15000;

int hatchSequenceStep = 0;

void startHatchSequence()
{
    hatchStatus = HatchStatus::OPENING;
}

// Opens and closes the hatch in steps of 10 degrees every 15 seconds.
// Call this function in void loop()
void updateHatchSequence()
{
    switch (hatchSequenceStep)
    {
    //////////////////
    ////// Idle //////
    //////////////////
    case 0:
        if (hatchStatus == HatchStatus::OPENING)
        {
            // Continue to the next step
            hatchSequenceStep += 1;
        }
        break;
    //////////////////
    /// Open Hatch ///
    //////////////////
    case 1:
        playHatchStepBuzz();
        hatchServoTargetPosition += 10;
        Serial.println("Moving the hatch to: " + String(hatchServoTargetPosition) + " degrees");

        hatchTimer = millis();
        hatchSequenceStep += 1;
        break;
    case 2:
        if ((millis() - hatchTimer) >= hatchStepDuration)
        {
            if (hatchServoTargetPosition >= 90)
            {
                // When the hatch is fully open, continue to the next step
                hatchSequenceStep += 1;
            }
            else
            {
                // If the hatch is not fully open, go back to the
                // previous step and keep opening
                hatchSequenceStep -= 1;
            }
        }
        break;
    case 3:
        hatchServoTargetPosition = 90;
        hatchStatus = HatchStatus::OPEN;
        Serial.println("Hatch is open!");
        hatchTimer = millis();
        hatchSequenceStep += 1;
        break;
    //////////////////
    /// Stay Open ////
    //////////////////
    case 4:
        if ((millis() - hatchTimer) >= hatchStepDuration)
        {
            hatchSequenceStep += 1;
        }
        break;
    //////////////////
    /// Close Hatch //
    //////////////////
    case 5:
        Serial.println("Closing hatch ...");
        hatchStatus = HatchStatus::CLOSING;
        hatchSequenceStep += 1;
        break;
    case 6:
        playHatchStepBuzz();
        hatchServoTargetPosition -= 10;
        Serial.println("Moving the hatch to: " + String(hatchServoTargetPosition) + " degrees");

        hatchTimer = millis();
        hatchSequenceStep += 1;
        break;
    case 7:
        if ((millis() - hatchTimer) >= hatchStepDuration)
        {
            if (hatchServoTargetPosition <= 0)
            {
                // When the hatch is fully closed, continue to the next step
                hatchSequenceStep += 1;
            }
            else
            {
                // If the hatch is not fully closed, go back to the
                // previous step and keep closing
                hatchSequenceStep -= 1;
            }
        }
        break;
    case 8:
        hatchServoTargetPosition = 0;
        hatchStatus = HatchStatus::CLOSED;
        Serial.println("Hatch is closed!");
        playHatchClosedBuzz();
        hatchSequenceStep = 0;
        break;
    }
}

/////////////////////////////////////
//////// Thermometer Code ///////////
/////////////////////////////////////

const int THERMOMETER_PIN = A0;

int temperature = 0;

// Reads the temperature from the thermometer.
// Call this function in void loop()
void updateThermometer()
{
    // Using a potentiometer to emulate the thermometer
    // Converting the potmeter value to a range of 0 to 30 degrees
    temperature = map(analogRead(THERMOMETER_PIN), 0, 1024, 0, 30);
}

// Run this code in void setup()
void setupThermometer()
{
    pinMode(THERMOMETER_PIN, INPUT);
}

/////////////////////////////////////
//////// Light Sensor Code //////////
/////////////////////////////////////

const int LIGHT_SENSOR_PIN = A1;

int lightLevel = 0;

// Reads the light level from the light sensor.
// Call this function in void loop()
void updateLightSensor()
{
    // Using a potentiometer to emulate the light sensor
    // Converting the potmeter value to a range of 0 to 100
    lightLevel = map(analogRead(LIGHT_SENSOR_PIN), 0, 1024, 0, 100);
}

// Run this code in void setup()
void setupLightSensor()
{
    pinMode(LIGHT_SENSOR_PIN, INPUT);
}

/////////////////////////////////////
//////////// RGB Code ///////////////
/////////////////////////////////////

const int RGB_RED_PIN = 6;
const int RGB_GREEN_PIN = 5;
const int RGB_BLUE_PIN = 3;

RgbMode rgbMode = RgbMode::OFF;

// https://www.wikihow.com/Convert-Degrees-to-Radians
double degreesToRadians(double degrees)
{
    return (degrees * PI) / 180.0;
}

// Generates a sine wave centered around the center value
// with a max=centerValue+amplitude and min=centerValue-amplitude
// The offset argument lets you add or remove time, and is
// useful if you need multiple sinewaves working together
// In 3 phase AC power for example there are 3 sine waves
// working at a 120 degree offset.
// https://en.wikipedia.org/wiki/Three-phase_electric_power#/media/File:3_phase_AC_waveform.svg
// https://www.google.com/search?q=generate+sine+wave+with+arduino
// Example: https://www.tinkercad.com/things/8wqREUCNY4p
double getSineWave(double amplitude, double frequency, double centerValue, double offset)
{
    // Creating a variable for the current moment in time
    // given in seconds. Using double as the datatype because
    // it can store massive numbers as decimals. The datatype
    // for the sin() function argument is also double.
    // https://reference.arduino.cc/reference/en/language/variables/data-types/double/
    double t = millis() / 1000.0;

    // How to use the sin() function: https://reference.arduino.cc/reference/tr/language/functions/trigonometry/sin/
    // The formula for a sine wave: https://en.wikipedia.org/wiki/Sine_wave
    return (amplitude * sin((2.0 * PI * frequency * t) + degreesToRadians(offset))) + centerValue;
}

void updateRgb()
{
    switch (rgbMode)
    {
    case RgbMode::OFF:
        analogWrite(RGB_RED_PIN, 0);
        analogWrite(RGB_GREEN_PIN, 0);
        analogWrite(RGB_BLUE_PIN, 0);
        break;
    case RgbMode::PULSE:
        // A quick google search shows that yellow is (R=255, G=255, B=0)
        // To pulse between red and yellow we just have to pulse the green color
        // https://www.google.com/search?q=rgb+values+for+yellow
        // After experimenting with the simulation,
        // the best value to produce orange is (R=255, G=40, B=0)

        analogWrite(RGB_RED_PIN, 255);
        analogWrite(RGB_GREEN_PIN, getSineWave(20.0, 0.5, 20.0, 0.0));
        analogWrite(RGB_BLUE_PIN, 0);
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
//////////// Main Code //////////////
/////////////////////////////////////

CatDX4001 cat;

int step = 0;

unsigned long case1LightTimer = 0;

unsigned long case2BuzzTimer = 0;
unsigned long case2BatchDestructionTimer = 0;

void setup()
{
    Serial.begin(9600);
    setupButtonA();
    setupCaseLedIndicator();
    setupHatchServo();
    setupThermometer();
    setupLightSensor();
    setupRgb();

    // Testing the code by activating it in void setup()
    rgbMode = RgbMode::PULSE;
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
        Serial.println("Press the confirmation button to continue");
        step += 1;
        break;
    case 1:
        if (buttonARisingEdge)
        {
            step += 1;
        }
        break;
    case 2:
        if (cat.get_status())
        {
            step += 1;
        }
        break;
    case 3:
        if (cat.select_operation(1))
        {
            step += 1;
        }
        break;
    case 4:
        if (lightLevel > 50)
        {
            Serial.println("Waiting for the light level to get below 50");
        }
        step += 1;
        break;
    case 5:
        if (lightLevel < 50)
        {
            step += 1;
        }
        break;
    case 6:
        Serial.println("Waiting 2 minutes for the batch settle");
        case1LightTimer = millis();
        step += 1;
        break;
    case 7:
    {
        // Reset the sequence to the start of the light level check
        if (lightLevel > 50)
        {
            step = 4;
        }

        // Continuing to the next step after 2 minutes
        if ((millis() - case1LightTimer) > (2 * 60e3))
        {
            step += 1;
        }
        break;
    }
    //////////////////
    ///// Case 2 /////
    //////////////////
    case 8:
        // Code that should run once during case 2
        setCaseLedIndicatorMode(LedIndicatorMode::CASE_2);
        playCaseChangeBuzz();

        Serial.println("Case 2 is active");
        Serial.println("Waiting for temperature to rise above 25 degrees");
        step += 1;
        break;
    case 9:
        if (temperature > 25)
        {
            Serial.println("Temperature is above 25 degrees");
            Serial.println("Press the confirmation button within 2 minutes or the batch will be destroyed");
            step += 1;
        }
        break;
    case 10:
        case2BuzzTimer = millis();
        case2BatchDestructionTimer = millis();
        step += 1;
        break;
    case 11:
    {
        // The code in this case is surrounded by curly braces {} to
        // create a new scope. This allows for the creation of local variables
        // inside the case.

        if (buttonARisingEdge)
        {
            stopBuzzer();
            step += 1;
        }

        unsigned long batchAge = millis() - case2BatchDestructionTimer;
        unsigned long timeSinceLastBuzz = millis() - case2BuzzTimer;

        // Using scientific notation to make the code more readable
        // 60e3 = 60 * 10^3 = 60 * 1000 = 60000 where 3 stands for 3 zeros (1000)
        if (batchAge <= 60e3)
        {
            if ((millis() - case2BuzzTimer) >= 10e3)
            {
                playCase2ShortBuzz();
                case2BuzzTimer = millis();
            }
        }
        else if (batchAge <= (60e3 + 30e3))
        {
            if (timeSinceLastBuzz >= 5e3)
            {
                playCase2ShortBuzzes(2);
                case2BuzzTimer = millis();
            }
        }
        else if (batchAge <= (60e3 + 30e3 + 15e3))
        {
            if (timeSinceLastBuzz >= 5e3)
            {
                playCase2ShortBuzzes(3);
                case2BuzzTimer = millis();
            }
        }
        else if (batchAge <= (2 * 60e3))
        {
            playCase2ConstantBuzz();
        }
        else
        {
            step = 0;
            stopBuzzer();
            Serial.println("Batch is destroyed!");
        }
        break;
    }
    //////////////////
    ///// Case 3 /////
    //////////////////
    case 12:
        // Code that should run once during case 3
        setCaseLedIndicatorMode(LedIndicatorMode::CASE_3);
        playCaseChangeBuzz();

        Serial.println("Case 3 is active");

        startHatchSequence();

        step += 1;
        break;
    case 13:
    {
        // Code that should run over and over again while
        // waiting for the hatch sequence to complete

        // The code in this case is surrounded by curly braces {} to
        // create a new scope. This allows for the creation of local variables
        // inside the case.
        bool hatchSequenceCompleted = hatchStatus == HatchStatus::CLOSED;

        // The hatch sequence gets updated by updateHatchSequence() in void loop()
        if (hatchSequenceCompleted)
        {
            Serial.println("Press the confirmation button to continue to step 4");
            step += 1;
        }
        break;
    }
    case 14:
        // Code that should run while waiting for the confirmation button press
        if (buttonARisingEdge)
        {
            step += 1;
        }
        break;
    //////////////////
    ///// Case 4 /////
    //////////////////
    case 15:
        // Code that should run once during case 4
        setCaseLedIndicatorMode(LedIndicatorMode::CASE_4);
        playCaseChangeBuzz();

        Serial.println("Case 4 is active");
        step += 1;
        break;
    case 16:
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
    updateThermometer();
    updateLightSensor();
    updateSequence();
    updateHatchSequence();
    updateHatchServo();
    updateCase2ShortBuzz();
    updateRgb();
}
