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
* This example uses a hall sensor and little magnets (3x1 mm) at the hands
* to make calibration possible.
*
* Note: in order to use the microstep mode with both motors, the adapted
* Arduino core is needed, otherwise changing the preScaler on timer 0
* will disturb micros() and millis()
*/

#include <Arduino.h>

#include "MotorVID28.h"

MotorVID28 motor(360*3*4, true, 9, 10, 11);
MotorVID28 motor2(360*3*4, true, 5, 6, 3);


void calibrate(MotorVID28 motor) {
  unsigned long now;
  unsigned int val, zero_position=0, max=0;
  unsigned int start_max=4319, end_max=0;

 max = 0;
 start_max=4319;
 end_max=0;
  for(int i=0; i<4320; i++) {
    val=analogRead(A3);
    if (val>max) {
      max=val;
      start_max = motor.getPosition();
    }
    if (val == max) {
      end_max = motor.getPosition();
    }
    motor.stepUp();
    now = micros();
    while ((micros() - now) < 2200 ) {
    }
  }
  unsigned int diff;
  diff = end_max - start_max;
  if (diff > 4320/2) { diff = diff + 4320; }
  zero_position = ((start_max + diff/2)%4320);
  motor.setPosition(zero_position);
  while(motor.isRunning()) motor.update();
  motor.setZero();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Enter a step position from 0 through ");
  Serial.print(4*360*3);
  Serial.println(".");

  setPrescaler(0, 1);
  setPrescaler(1, 1);
  setPrescaler(2, 1);

  calibrate(motor);
  calibrate(motor2);
}


static int nextPos = 0;
void loop(void)
{
  // the motor only moves when you call update()
  motor.update();
  motor2.update();

  if (Serial.available()) {
    char c = Serial.read();
    if (c=='\n') {
      motor.setPosition(nextPos);
      motor2.setPosition((nextPos+2160));
      nextPos = 0;
    } else if (c>='0' && c<='9') {
      nextPos = 10*nextPos + (c-'0');
    } else if (c=='z') {
      motor.setPosition(0);
      motor2.setPosition(0);
    } else if (c=='r') {
      calibrate(motor);
      calibrate(motor2);
    } else if (c=='s') {
      motor.setMaxSpeed(500);
      motor2.setMaxSpeed(1000);
    } else if (c=='S') {
      motor.setMaxSpeed(250);
      motor2.setMaxSpeed(500);
    } else if (c=='f') {
      motor.setMaxSpeed(2000);
      motor2.setMaxSpeed(2000);
    }
  }
}
