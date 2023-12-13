# fanpwm
Arduino Driving a 4-Pin Noctua 12v Fan

Despite the instructions on Noctua's 'White Paper' - I can drive the Fan Speed with a very low frequency PWM. I think the Arduino Nano Mini Pro has a PWM freq of 490 Hz!

I've included the source code here, as it contains a segment dedicated to managing a rotary encoder's input for adjusting the fan speed. This implementation cleverly sidesteps the necessity for a hardware low-pass filter by integrating a Finite State Machine (FSM) to address unwanted noise.

The FSM is adept at distinguishing between anticlockwise and clockwise rotations, effectively and efficiently disregarding noise and debouncing issues.

https://johnwilsonpersonalportfolio.wordpress.com/2020/07/08/noctua-pwm-fan-control-with-a-mini-pro-and-rotary-encoder/
