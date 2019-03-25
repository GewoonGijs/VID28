# VID28

This Library controls VID28 and equivalent steppermotors (X25, BKA30D-R5, VID29). 

It can control a stepper either using 6 steps mode or microsteps mode (24 steps) using PWM.

If usin PWM, it is necessary to set the prescaler of the corresponding timer to 1 to enforce high frequency PWM. Otherwise you can hear the stepper motor peep.

It can be used standalone or together with the AccelStepper library (see example).

If you have any questions, don't hesitate to contact me.

If you need to control two steppers with one arduino, you need 6 PWM outputs. That means you have to change the prescaler of timer0 as well Without any changes, this wil disturbe the behaviour of micros() and millis(). Ther is a solution for that in the adapted arduino_framewrok (see https://github.com/GewoonGijs/platformio-pkg-framework-arduinoavr and the adapted file wiring.c in /cores/arduino/

Have fun!!!
