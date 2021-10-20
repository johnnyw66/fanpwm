# fanpwm
Arduino Driving a 4-Pin Noctua 12v Fan

Despite the instructions on Noctua's 'White Paper' - I can drive the Fan Speed with a very low frequency PWM. I think the Arduino Nano Mini Pro has a PWM freq of 490 Hz!

I've included the source here, because there's some code which handles a rotary encoder (for speeding/slowing down the fan) by implementing a finite state machine (FSM).

The FSM can recognise an anticlockwise or clockwise rotation - effectively and efficently ignoring noise/debounce. 


https://johnwilsonpersonalportfolio.wordpress.com/2020/07/08/noctua-pwm-fan-control-with-a-mini-pro-and-rotary-encoder/
