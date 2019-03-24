#include <Arduino.h>
#include "AccelStepper.h"

#include "MotorVID28.h"

MotorVID28 motor(360*3*4, true, 9, 10, 11);

// Create callback handlers, and instantiate an AccelStepper object

void stepper_fw() {
  motor.stepUp();
}
void stepper_bw() {
  motor.stepDown();
}
AccelStepper stepper(stepper_fw, stepper_bw);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Enter a step position from 0 through ");
  Serial.print(4*360*3);
  Serial.println(".");

  setPrescaler(1, 1);
  setPrescaler(2, 1);

  stepper.setMaxSpeed(1500.0);
  stepper.setAcceleration(75.0);
}

void loop(void)
{
  static int nextPos = 0;
  // the motor only moves when you call run()
  stepper.run();

  if (Serial.available()) {
    char c = Serial.read();
    if (c==10 || c==13) {
      //motor1.setPosition(nextPos);
      stepper.moveTo(nextPos);
      nextPos = 0;
    } else if (c>='0' && c<='9') {
      nextPos = 10*nextPos + (c-'0');
    } else if (c=='t') {
      Serial.println("Starttime");
      Serial.print(millis());
      Serial.print(" - ");
      Serial.println(micros());

      stepper.runToNewPosition(stepper.currentPosition()+4320);
      while (stepper.distanceToGo() > 0){
        stepper.run();
      }

      Serial.print(millis());
      Serial.print(" - ");
      Serial.println(micros());
    }
  }
}
