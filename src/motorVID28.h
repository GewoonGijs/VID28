#ifndef MotorVID29_h
#define MotorVID29_h
#include <Arduino.h>

class MotorVID29
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

   MotorVID29(unsigned int steps, boolean microstepmode, char pin1, unsigned char pin2, unsigned char pin3);

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
