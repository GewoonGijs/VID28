
//
//
//  Use 3 PWM pins to drive motor
//
// Choose 9,10,11
//

#include <Arduino.h>
#include "MotorVID29.h"


MotorVID29 motor1(360*3*4, true, 9, 10, 11);
MotorVID29 motor2(360*3*4, true, 3, 5, 6);

int i = 0;

void setup() {
  // put your setup code here, to run once:

  motor1.setPosition(0);
  motor2.setPosition(0);

  Serial.begin(9600);
  Serial.print("Enter a step position from 0 through ");
  Serial.print(360*3*4-1);
  Serial.println(".");

}

void loop(void)
{
  static int nextPos = 0;
  // the motor only moves when you call update
  motor1.update();
  motor2.update();

  if (Serial.available()) {
    char c = Serial.read();
    if (c==10 || c==13) {
      motor1.setPosition(nextPos);
      motor2.setPosition((nextPos + 360*3*2) % 360*3*4 ); //opposite direction
      nextPos = 0;
    } else if (c>='0' && c<='9') {
      nextPos = 10*nextPos + (c-'0');
    }
  }
}
