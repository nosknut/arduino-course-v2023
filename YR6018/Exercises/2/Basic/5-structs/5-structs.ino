// Simulation:
// https://wokwi.com/projects/368604124891332609

// A recipe for non-blocking arduino code can be found here:
// https://github.com/nosknut/arduino-course-v2023/blob/main/IELET1002/ArduinoExampleCode/FromBlockingToNonBlocking/FromBlockingToNonBlocking.ino

#include "Led.h"
#include "Rgb.h"
#include "getSquareWave.h"
#include "getTriangleWave.h"
#include "WinnerLed.h"
#include "LooserLed.h"
#include "WinnerCelebration.h"
#include "LooserHumiliation.h"
#include "Buzzer.h"
#include "Button.h"
#include "GameSequence.h"

Rgb rgb;
Led led1;
Led led2;

WinnerLed winnerLed;
LooserLed looserLed;

WinnerCelebration winnerCelebration;
LooserHumiliation looserHumiliation;

Buzzer buzzer;

Button button1;
Button button2;

GameSequence gameSequence;

void setup()
{
    Serial.begin(9600);

    randomSeed(analogRead(0));

    rgb.redPin = 9;
    rgb.greenPin = 8;
    rgb.bluePin = 7;
    rgb.setup();

    led1.pin = 6;
    led1.setup();

    led2.pin = 10;
    led2.setup();

    button1.pin = 2;
    button1.setup();

    button2.pin = 12;
    button2.setup();

    buzzer.pin = 11;
    buzzer.setup();
}

void loop()
{
    button1.update();
    button2.update();

    gameSequence.update(
        button1,
        button2,
        winnerLed,
        looserLed,
        winnerCelebration,
        looserHumiliation,
        buzzer,
        rgb);

    winnerCelebration.update(
        winnerLed,
        looserLed,
        buzzer,
        rgb,
        led1,
        led2);

    looserHumiliation.update(
        winnerLed,
        looserLed,
        buzzer,
        rgb,
        led1,
        led2);

    winnerLed.update(led1, led2, rgb);
    looserLed.update(led1, led2, rgb);

    buzzer.update();
}
