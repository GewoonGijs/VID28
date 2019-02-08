
//
//
//  Use 3 PWM pins to drive motor
//
// Choose 9,10,11
//

#include <Arduino.h>
#include "MotorVID29.h"


void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}


MotorVID29 motor1(360*3*4, true, 11, 10, 9);
MotorVID29 motor2(360*3*4, true, 5, 6, 3);

int i = 0;

void setup() {
  // put your setup code here, to run once:


  setPwmFrequency(9, 1);
  setPwmFrequency(10, 1);
  setPwmFrequency(11, 1);
  setPwmFrequency(6, 1);
  setPwmFrequency(5, 1);
  setPwmFrequency(3, 1);

  motor1.setPosition(0);
  motor2.setPosition(0);

  Serial.begin(9600);
  Serial.print("Enter a step position from 0 through ");
  Serial.print(360*3-1);
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
      motor2.setPosition((nextPos + 180*3*4)); //opposite direction
      Serial.print(motor1.targetStep);
      Serial.print(" - ");
      Serial.println(motor2.targetStep);
      nextPos = 0;
    } else if (c>='0' && c<='9') {
      nextPos = 10*nextPos + (c-'0');
    } else if (c=='t') {
      for(int i = 0; i<360*3*4; i++){
        motor1.stepUp();
        motor2.stepDown();
        delayMicroseconds(600);
      }
    }
  }
}
