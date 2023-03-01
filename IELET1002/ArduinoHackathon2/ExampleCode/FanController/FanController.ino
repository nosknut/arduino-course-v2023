///////////////////////////////////////////////////////
//////////////////// Common ///////////////////////////
///////////////////////////////////////////////////////

int motorState = 0;

///////////////////////////////////////////////////////
//////////////////// Motor ////////////////////////////
///////////////////////////////////////////////////////

// Pin to drive motor counterclockwise, when set to true
const int MOTOR_CCW_PIN = 12;

// Motor clockwise, when true
const int MOTOR_CW_PIN = 14;

// Motor speed
const int MOTOR_ENABLE_PIN = 27;

void writeMotorSpeed(int speed)
{
    // Set the direction
    if (speed < 0)
    {
        // Negative speeds set motor clockwise
        digitalWrite(MOTOR_CW_PIN, HIGH);
        digitalWrite(MOTOR_CCW_PIN, LOW);
    }

    if (speed > 0)
    {
        // Positive speeds set motor counterclockwise
        digitalWrite(MOTOR_CW_PIN, LOW);
        digitalWrite(MOTOR_CCW_PIN, HIGH);
    }

    if (speed == 0)
    {
        // Motor stop
        digitalWrite(MOTOR_CW_PIN, LOW);
        digitalWrite(MOTOR_CCW_PIN, LOW);
    }

    // Set the speed
    analogWrite(MOTOR_ENABLE_PIN, speed);
}

void updateMotorSpeed()
{
    switch (motorState)
    {
    // Stopped
    case 0:
        writeMotorSpeed(0);
        break;
    // Slow
    case 1:
        writeMotorSpeed(100);
        break;
    // Medium
    case 2:
        writeMotorSpeed(200);
        break;
    // Fast
    case 3:
        writeMotorSpeed(255);
        break;
    }
}

void increaseMotorState()
{
    if (motorState < 3)
    {
        motorState += 1;
    }
}

void decreaseMotorState()
{
    if (motorState > 0)
    {
        motorState -= 1;
    }
}

void setupMotor()
{
    pinMode(MOTOR_CCW_PIN, OUTPUT);
    pinMode(MOTOR_CW_PIN, OUTPUT);
    pinMode(MOTOR_ENABLE_PIN, OUTPUT);
}

///////////////////////////////////////////////////////
//////////////////// Buttons //////////////////////////
///////////////////////////////////////////////////////

const int SPEED_UP_BUTTON_PIN = 24;
const int SPEED_DOWN_BUTTON_PIN = 25;

// Are the buttons currently pressed?
bool speedUpButtonState = false;
bool speedDownButtonState = false;

// Time till next buttenpress will be registered
int debounceDelay = 50;

unsigned long speedUpButtonTimer = 0;
unsigned long speedDownButtonTimer = 0;

void updateSpeedUpButtonState()
{
    // if click is registered within the debounceDelay time, skip rest of function
    if ((millis() - speedUpButtonTimer) < debounceDelay)
        return;

    // newState = true if SPEED_UP_BUTTON is pressed
    bool newState = digitalRead(SPEED_UP_BUTTON_PIN);

    // New timer if button state changes
    if (newState != speedUpButtonState)
        speedUpButtonTimer = millis();

    speedUpButtonState = newState;
}

void updateSpeedDownButtonState()
{
    // if click is registered within the debounceDelay time, skip rest of function
    if ((millis() - speedDownButtonTimer) < debounceDelay)
        return;

    // newState = true if SPEED_DOWN_BUTTON is pressed
    bool newState = digitalRead(SPEED_DOWN_BUTTON_PIN);

    // New timer if button state changes
    if (newState != speedDownButtonState)
        speedDownButtonTimer = millis();

    speedDownButtonState = newState;
}

void setupButtons()
{
    pinMode(SPEED_UP_BUTTON_PIN, INPUT);
    pinMode(SPEED_DOWN_BUTTON_PIN, INPUT);
}

void updateButtonStates()
{
    updateSpeedUpButtonState();
    updateSpeedDownButtonState();
}

///////////////////////////////////////////////////////
/////////////// Button Edge detection /////////////////
///////////////////////////////////////////////////////

// The button states from the previous loop
bool previousSpeedUpButtonState = false;
bool previousSpeedDownButtonState = false;

// Become true when the puttons are pressed
bool speedUpButtonPressed = false;
bool speedDownButtonPressed = false;

void updateSpeedUpButtonPressed()
{
    // Sets the variable speedUpButtonPressed to true if the speedUpButtonState
    // changes from 0 to 1.
    if ((speedUpButtonState == true) && (previousSpeedUpButtonState == false))
    {
        speedUpButtonPressed = true;
        Serial.println("speedUpButton pressed");
    }
    else
        speedUpButtonPressed = false;

    previousSpeedUpButtonState = speedUpButtonState;
}

void updateSpeedDownButtonPressed()
{
    // Sets the variable speedDownButtonPressed to true if the speedDownButtonState
    // changes from 0 to 1.
    if ((speedDownButtonState == true) && (previousSpeedDownButtonState == false))
    {
        speedDownButtonPressed = true;
        Serial.println("speedDownButton pressed");
    }
    else
        speedDownButtonPressed = false;

    previousSpeedDownButtonState = speedDownButtonState;
}

void updateButtonPressedStates()
{
    updateSpeedUpButtonPressed();
    updateSpeedDownButtonPressed();
}

///////////////////////////////////////////////////////
//////////////////// Logic ////////////////////////////
///////////////////////////////////////////////////////

void updateMotorSpeedControls()
{
    if (speedUpButtonPressed)
    {
        increaseMotorState();
        Serial.println("Increasing speed");
    }
    if (speedDownButtonPressed)
    {
        decreaseMotorState();
        Serial.println("Decreasing speed");
    }
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

void setup()
{
    Serial.begin(115200);

    setupMotor();
    setupButtons();
}

void loop()
{
    updateButtonStates();
    updateButtonPressedStates();
    updateMotorSpeedControls();
    updateMotorSpeed();
}