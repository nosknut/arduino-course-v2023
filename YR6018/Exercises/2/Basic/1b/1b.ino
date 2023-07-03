// Simulation:
// https://wokwi.com/projects/368604124891332609

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

void updateButton1State()
{
    // Because the button is wired as a pullup circuit,
    // the pin will detect a LOW signal when the button is pressed,
    // and a HIGH signal when the button is not pressed.
    button1State = digitalRead(BUTTON_1_PIN) == LOW;
}

void setupButton1()
{
    pinMode(BUTTON_1_PIN, INPUT_PULLUP);
}

/////////////////////////////////////
/////// Button 2 State Code /////////
/////////////////////////////////////

const int BUTTON_2_PIN = 12;

bool button2State = false;

void updateButton2State()
{
    // Because the button is wired as a pullup circuit,
    // the pin will detect a LOW signal when the button is pressed,
    // and a HIGH signal when the button is not pressed.
    button2State = digitalRead(BUTTON_2_PIN) == LOW;
}

void setupButton2()
{
    pinMode(BUTTON_2_PIN, INPUT_PULLUP);
}

/////////////////////////////////////
/////// Button Test Code ////////////
/////////////////////////////////////

// Allows for manual testing of the buttons and LED's by writing
// the button states to their respective LED's.
void updateTestButtons()
{
    digitalWrite(LED_1_PIN, button1State);
    digitalWrite(LED_2_PIN, button2State);
}

/////////////////////////////////////
/////////// Main Code ///////////////
/////////////////////////////////////

void setup()
{
    setupLeds();
    setupButton1();
    setupButton2();
}

void loop()
{
    updateButton1State();
    updateButton2State();

    updateTestButtons();
}
