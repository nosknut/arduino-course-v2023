#include <Servo.h>
#include "CatDX4001.h"
#include "Button.h"
#include "Buzzer.h"
#include "Hatch.h"
#include "LedIndicator.h"
#include "Rgb.h"
#include "ServoPositionController.h"
#include "Thermometer.h"
#include "LightSensor.h"
#include "DryBlendSequence.h"
#include "MainSequence.h"

// https://wokwi.com/projects/368049679345794049

// A recipe for non-blocking arduino code can be found here:
// https://github.com/nosknut/arduino-course-v2023/blob/main/IELET1002/ArduinoExampleCode/FromBlockingToNonBlocking/FromBlockingToNonBlocking.ino

CatDX4001 cat;

Button buttonA;
Buzzer buzzer;
LedIndicator ledIndicator;
Rgb rgb;
Thermometer thermometer;
LightSensor lightSensor;

Hatch hatch;

Servo hatchServo;
ServoPositionController hatchServoPositionController;

MainSequence mainSequence;
DryBlendSequence dryBlendSequence;

void setup()
{
    Serial.begin(9600);

    buttonA.pin = 2;
    buttonA.setup();

    ledIndicator.led1Pin = 4;
    ledIndicator.led2Pin = 7;
    ledIndicator.led3Pin = 8;
    ledIndicator.led4Pin = 9;
    ledIndicator.setup();

    buzzer.pin = 10;
    buzzer.setup();

    hatchServo.attach(11);

    thermometer.pin = A0;
    thermometer.setup();

    lightSensor.pin = A1;
    lightSensor.setup();

    rgb.redPin = 6;
    rgb.greenPin = 5;
    rgb.bluePin = 3;
    rgb.setup();
}

void loop()
{
    buttonA.update();
    thermometer.update();
    lightSensor.update();

    // https://www.w3schools.com/cpp/cpp_function_reference.asp
    dryBlendSequence.update(buzzer, rgb);

    hatch.updateHatchSequence(buzzer, hatchServoPositionController);

    mainSequence.update(
        buzzer,
        ledIndicator,
        buttonA,
        cat,
        lightSensor,
        hatch,
        dryBlendSequence,
        thermometer);

    hatchServoPositionController.update();
    hatchServo.write(hatchServoPositionController.position);

    buzzer.update();
    rgb.update();
}
