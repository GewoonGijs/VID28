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
#ifndef MotorVID28_h
#define MotorVID28_h
#include <Arduino.h>
#include "util/util.h"

class MotorVID28
{
 public:
   static const unsigned char pinCount = 3;
   unsigned char stateCount;      // 6 states or 24 microstates
   unsigned char pins[pinCount];
   unsigned char currentState;    // current state in the cycle
   unsigned int currentStep;      // step we are currently at
   unsigned int targetStep;       // target we are moving to
   unsigned int steps;            // total steps available
   unsigned long time0;           // time when we entered this state
   unsigned int microDelay;       // microsecs until next state
   char  dir;                     // direction -1,0,1
   boolean stopped;               // true if stopped
   boolean microstepmode;         // true for MicroStep, false for PartialStep

   MotorVID28(unsigned int steps, boolean microstepmode, char pin1, unsigned char pin2, unsigned char pin3);

   void stepUp();
   void stepDown();
   void update();
   void setPosition(unsigned int pos);
   void setPosition(unsigned int pos, signed char dir);
   void powerOff();

 private:
   void advance();
   void writeIO();
};


#endif
