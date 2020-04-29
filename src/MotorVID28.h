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

#define MINIMUM_DELAY 500 //Minimum delay in microsecs.

class MotorVID28
{
 public:
   MotorVID28(unsigned int steps, boolean microstepmode, char pin1, unsigned char pin2, unsigned char pin3);

   void stepUp();
   void stepDown();
   void update();
   void setPosition(unsigned int pos);
   void setPosition(unsigned int pos, signed char dir);
   unsigned int getPosition() { return currentStep; }
   boolean isRunning() { return (!stopped); }
   void powerOff();

   void setZero() { currentStep = 0; }  //Used in conjuction with a magnet and a Hall sensor.
   void setMaxSpeed(unsigned int velocity) { minDelay = (velocity != 0) ? 1000000 / velocity : MINIMUM_DELAY ; } //velocity in ticks / second

 private:
   void advance();
   void writeIO();

   static const unsigned char pinCount = 3;
   unsigned char stateCount;      // 6 states or 24 microstates
   unsigned char pins[pinCount];
   unsigned char currentState;    // current state in the cycle
   unsigned int currentStep;      // step we are currently at
   unsigned int targetStep;       // target we are moving to
   unsigned int stepSinceStart;   // steps since last start, used for accel/decel
   unsigned int steps;            // total steps available
   unsigned long time0;           // time when we entered this state
   unsigned int microDelay;       // microsecs until next state
   unsigned int minDelay;         // Minimum time between steps determined by setMaxSpeed().
   char  dir;                     // direction -1,0,1
   boolean stopped;               // true if stopped
   boolean microstepmode;         // true for MicroStep, false for PartialStep

};


#endif
