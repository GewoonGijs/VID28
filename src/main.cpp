
//
//
//  Use 3 PWM pins to drive motor
//
// Choose 9,10,11 and 6,5,3
//
// Had to change the prescaler wit setPwmFrequency, @TODO need to clean
// this up. And have a challenge to get a repaired micros() and millis().
// because I would like to use PJON for a 1 wire communication bus.
//

#include <Arduino.h>
#include "AccelStepper.h"

#include "MotorVID29.h"

void setPrescaler(int timer, int divisor) {
  byte mode;
  if(timer == 0 || timer == 1) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(timer == 0) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(timer == 2) {
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
void stepper1_fw() {
  motor1.stepUp();
}
void stepper1_bw() {
  motor1.stepDown();
}
AccelStepper stepper1(stepper1_fw, stepper1_bw);

int i = 0;

void setup() {

  motor1.setPosition(0);
  motor2.setPosition(0);

  Serial.begin(9600);
  Serial.print("Enter a step position from 0 through ");
  Serial.print(4*360*3);
  Serial.println(".");

  // put your setup code here, to run once:

  setPrescaler(0, 1);
  setPrescaler(1, 1);
  setPrescaler(2, 1);

  stepper1.setMaxSpeed(1800.0);
  stepper1.setAcceleration(600.0);
}


void loop(void)
{
  static int nextPos = 0;
  // the motor only moves when you call update
  //motor1.update();
  stepper1.run();
  motor2.update();

  if (Serial.available()) {
    char c = Serial.read();
    if (c==10 || c==13) {
      //motor1.setPosition(nextPos);
      stepper1.moveTo(nextPos);
      motor2.setPosition((nextPos + 180*3*4)); //opposite direction
      Serial.print(motor1.targetStep);
      Serial.print(" - ");
      Serial.println(motor2.targetStep);
      nextPos = 0;
    } else if (c>='0' && c<='9') {
      nextPos = 10*nextPos + (c-'0');
    } else if (c=='t') {
      Serial.println("Starttime");
      Serial.print(millis());
      Serial.print(" - ");
      Serial.println(micros());

      stepper1.runToNewPosition(stepper1.currentPosition()+4320);
      while (stepper1.distanceToGo() > 0){
        stepper1.run();
      }

      Serial.print(millis());
      Serial.print(" - ");
      Serial.println(micros());

      for(int i = 0; i<360*3*4; i++){
        //motor1.stepUp();
        motor2.stepDown();
        delayMicroseconds(500);
      }
      Serial.println("Endtime");
      Serial.print(millis());
      Serial.print(" - ");
      Serial.println(micros());
    }
  }
}
