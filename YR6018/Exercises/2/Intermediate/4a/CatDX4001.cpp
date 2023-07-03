/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#include "CatDX4001.h"
#include "Arduino.h"

long SECOND = 1000;



CatDX4001::CatDX4001()
{
  _prepare_time = 5000;
}

bool CatDX4001::get_status()
{
  if (millis() < _prepare_time) {
    Serial.println("System is still preparing");
    return 0;
  } 


  
  Serial.println("Please make your selection of operations");
  Serial.println("(1) - Add catalyst");
  Serial.println("(2) - Heat up compound in target chamber (Not available for this compound)");
  Serial.println("(3) - Run centrifuge on compound (Not available for this compound");    

  return 1;
}

bool CatDX4001::select_operation(int desired_number){
  Serial.println("Selected " + String(desired_number));
  if (desired_number < 1 || desired_number > 3) {
    Serial.println("Please select one of the available catalysts");
    return 0;
  }
  // Hard coded bs, bare for serialprint
  if (desired_number == 1) {
    Serial.println("Adding catalyst");
  }
  if (desired_number == 2) {
    Serial.println("This unit is not available.");
    return 0;
    Serial.println("Heating compound. Place compound in heating chamber.");
    Serial.println("Press 1 to confirm.");
  }
  
  if (desired_number == 3) {
    Serial.println("This unit is not available.");
    return 0;
    Serial.println("Running centrifuge");
  }
  Serial.println("Operation will take 60 seconds");
  long _time_to_wait = millis() + 60*SECOND;
  while (_time_to_wait > millis());
  Serial.println("Complete");
  Serial.println("Please allow the system 10 seconds to reset.");
  _prepare_time = millis() + 10*SECOND;
  return 1;
  
}
