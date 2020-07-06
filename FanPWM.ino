//GPIO for A and B Rotary Encoder pins
#define ENCODER_INPUT_A 5
#define ENCODER_INPUT_B 6 
#define FAN_PWMPIN 3
#define LED_ROTHIGH 7
#define LED_ROTLOW 8

#define MINROTVALUE 1    
#define MAXROTVALUE 99

#define FANSPEED_INITICATOR_DELAY 50 



#define STATE_00    0
#define STATE_C01   1
#define STATE_C10   2
#define STATE_C11   3
#define STATE_A01   5
#define STATE_A10   6
#define STATE_A11   7

#define DIR_X  0x00    // Don't Know
#define DIR_C  0x10    // Clockwise
#define DIR_A  0x20    // ANTI clockwise



typedef struct _fsm_node {
    int nextState ;
    int dir ;
} FSMNode ;

#define _FSE(_dir,_ns)  {nextState:_ns, dir:_dir}
#define _HSE(_dir,_ns)  {nextState:_ns, dir:DIR_X}
#define _SE(_dir, _ns)  {nextState:_ns, dir:DIR_X}
#define _RESET(_ns) {nextState:_ns, dir:DIR_X}
 
 
int state = 0 ;


// Rotary Encoder FSM table -  8 States by 2 bit input (32 FSM nodes)
// This table can easily be fitted into an array of 8x4 bytes (high nybble has direction, low nybble the 'next state')
// To do this we need to refine our macros. For example #define _FSE(_dir,_ns)  (_dir | _ns)
FSMNode fsm[8][4] = {
       //                                 Pins 'BA'
       //          00                     01                   10             11
       {_RESET(STATE_00),  _SE(DIR_C,STATE_C01), _SE(DIR_A,STATE_A10), _RESET(STATE_00)},  //STATE_00
        
       //          00                     01                   10             11
       {_SE(DIR_A,STATE_00), _RESET(STATE_C01), _RESET(STATE_C10), _SE(DIR_C,STATE_C11)},  //STATE_C01
       
       //          00                     01                   10             11
       {_FSE(DIR_C,STATE_00), _RESET(STATE_C01), _RESET(STATE_C10), _SE(DIR_A,STATE_A11)},  //STATE_C10
       
       //          00                     01                   10             11
       {  _RESET(STATE_00),     _HSE(DIR_A,STATE_A01), _HSE(DIR_C,STATE_C10), _RESET(STATE_C11)}, //STATE_C11
       
       {0,0,0},      //4 Not used
       
       //          00                     01                   10             11
       {_FSE(DIR_A,STATE_00), _RESET(STATE_A01), _RESET(STATE_A10), _SE(DIR_C,STATE_C11)}, //STATE_A01
       
       //          00                     01                   10             11
       {_SE(DIR_C,STATE_00),   _RESET(STATE_A01), _RESET(STATE_A10), _SE(DIR_A,STATE_A11)}, //STATE_A10

       //          00                     01                   10             11
       {_RESET(STATE_00),  _HSE(DIR_A,STATE_A01), _HSE(DIR_C,STATE_C10), _RESET(STATE_A11)}, //STATE_A11
        
     
} ;


bool handleRotaryEncoder(void) ;


int rotValue = MINROTVALUE ;

void setup() {

  Serial.begin(9600) ;
  pinMode(ENCODER_INPUT_A, INPUT_PULLUP) ;
  pinMode(ENCODER_INPUT_B, INPUT_PULLUP) ;

  pinMode(FAN_PWMPIN, OUTPUT);
  
  
  pinMode(LED_ROTHIGH, OUTPUT);
  digitalWrite(LED_ROTHIGH,HIGH) ;
  pinMode(LED_ROTLOW, OUTPUT);
  digitalWrite(LED_ROTLOW,HIGH) ;
  
  setFanPWM(rotValue) ;

  for(int i = 0 ; i < 5 ; i++) 
    fanLEDBlink(FANSPEED_INITICATOR_DELAY, LED_ROTLOW) ;
    
  for(int i = 0 ; i < 5 ; i++) 
    fanLEDBlink(FANSPEED_INITICATOR_DELAY, LED_ROTHIGH) ;


}

void fanLEDBlink(int del,int indicator){
    digitalWrite(indicator,LOW) ;
    delay(del) ;
    digitalWrite(indicator,HIGH) ;
    delay(del) ;
}



int getRotaryDirection() {

  int  encoderInput = digitalRead(ENCODER_INPUT_B) << 1 | digitalRead(ENCODER_INPUT_A);

  FSMNode fsmNode = fsm[state][encoderInput] ;
  state = fsmNode.nextState ;
  return fsmNode.dir ;

}

void loop() {
    
    if (handleRotaryEncoder()) {
      setFanPWM(rotValue) ;
    }

    digitalWrite(LED_ROTHIGH,((rotValue == MAXROTVALUE) ? LOW : HIGH)) ;
    digitalWrite(LED_ROTLOW,((rotValue == MINROTVALUE) ? LOW : HIGH)) ;
    
} 

bool handleRotaryEncoder() {
  int dirct = getRotaryDirection() ;
  
  if (dirct == DIR_C) {
    
    if (rotValue < MAXROTVALUE) {
      rotValue++;
      return true ;
    }

  } else if (dirct == DIR_A) {
    if (rotValue > MINROTVALUE) {
        rotValue--;
        return true ;
       
    }
  } 
  return false ;
}

 void setFanPWM(int dCycle) {
      Serial.println(dCycle) ;
      int sdc = map(dCycle, MINROTVALUE, MAXROTVALUE, 0, 255);
      analogWrite(FAN_PWMPIN, sdc);
}
