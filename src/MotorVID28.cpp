/*
 * Copyright (c) 2019 Gijs Withagen.
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
*/
//
//
// The VID29 is a 6 state stepper motor. Pin 2 and 3 of the stepper motor
// are high and low simultaneously. So they share one IO pin.
//
// Note: to use microsteps we need to assing PWM capable outputs

#include <Arduino.h>
#include "MotorVID28.h"

// State  2 1 0   Value
// 0      1 0 1   0x5
// 1      0 0 1   0x1
// 2      0 1 1   0x3
// 3      0 1 0   0x2
// 4      1 1 0   0x6
// 5      1 0 0   0x4
static byte stateMap[] = {0x5, 0x1, 0x3, 0x2, 0x6, 0x4};

// Divdide each step in 4 microsteps makes 24 steps per cycle
static byte microStepState[] = {251, 238, 218, 191,
                                160, 128, 95, 64,
                                37, 17, 4, 0,
                                4, 17, 37, 64,
                                95, 128, 160, 191,
                                218, 238, 251, 255};

#define STARTINDEX_PIN1 18 // 0 // 23-5
#define STARTINDEX_PIN23 10 // 23-13
#define STARTINDEX_PIN4 2 // 23-21

#define STEPTIME 800  // 800 microsecs between steps

MotorVID28::MotorVID28(unsigned int steps, boolean microstepmode, char pin1, unsigned char pin2, unsigned char pin3)
{
  this->currentState = 0;
  this->steps = steps;
  this->microstepmode = microstepmode;
  this->stateCount = (microstepmode ? 24 : 6 );
  this->pins[0] = pin1;
  this->pins[1] = pin2;
  this->pins[2] = pin3;
  for (int i=0;i<pinCount;i++) {
    pinMode(pins[i], OUTPUT);
  }
  dir = 0;
  stopped = true;
  currentStep = 0;
  targetStep = 0;
}


void MotorVID28::writeIO()
{
  if (microstepmode) {
    analogWrite(pins[0], microStepState[(currentState+STARTINDEX_PIN1) % stateCount]);
    analogWrite(pins[1], microStepState[(currentState+STARTINDEX_PIN23) % stateCount]);
    analogWrite(pins[2], microStepState[(currentState+STARTINDEX_PIN4) % stateCount]);
  } else {
    byte mask = stateMap[currentState];
    for (int i=0;i<pinCount;i++) {
      digitalWrite(pins[i], mask & 0x1);
      mask >>= 1;
    }
  }
}

void MotorVID28::powerOff()
{
  for (int i=0;i<pinCount;i++) {
    digitalWrite(pins[i], 0);
  }
}

void MotorVID28::stepUp()
{
  currentStep = (currentStep + 1) % steps ;
  currentState = (currentState + 1) % stateCount;
  writeIO();
}

void MotorVID28::stepDown()
{
  currentStep = (currentStep + steps - 1) % steps ;
  currentState = (currentState + (stateCount - 1)) % stateCount;
  writeIO();
}

void MotorVID28::advance()
{
  // detect stopped state
  if (currentStep==targetStep) {
    stopped = true;
    dir = 0;
    time0 = micros();
    return;
  }

  if (dir>0) {
    stepUp();
  } else {
    stepDown();
  }

  microDelay = STEPTIME;
  time0 = micros();
}

void MotorVID28::setPosition(unsigned int pos)
{
  // pos is unsigned so don't need to check for <0
  targetStep = pos % steps;
  dir = currentStep<targetStep ? 1 : -1;
  if (stopped) {
    // reset the timer to avoid possible time overflow giving spurious deltas
    stopped = false;
    time0 = micros();
    microDelay = 0;
  }
}

void MotorVID28::setPosition(unsigned int pos, signed char d)
{
  setPosition(pos);
  dir = d; //overrule direction as set in setPosition
}

void MotorVID28::update()
{
  if (!stopped) {
    unsigned long delta = micros() - time0;
    if (delta >= microDelay) {
      advance();
    }
  }
}
