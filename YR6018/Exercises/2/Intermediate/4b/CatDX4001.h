/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef CatDX4001_h
#define CatDX4001_h

#include "Arduino.h"

class CatDX4001
{
  public:
    CatDX4001();
    bool get_status();
    bool select_operation(int);
  private:
    int _status;
    long _prepare_time;

};

#endif
