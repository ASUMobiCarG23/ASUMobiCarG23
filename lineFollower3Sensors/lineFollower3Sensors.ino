#define LS 4 //left sensor
#define CS 6 //center sensror
#define RS 7 //right sensor
int state = 0;// state = 0 goforward
              // state = 1 goAheadRight
              // state = 2 goAheadLeft
              // state = 3 goHardRight
              // state = 4 goHardLeft

void setup() {
  
  pinMode(LS , INPUT);
  pinMode(RS , INPUT);

}


void loop() {

  //Take Decision
  switch (state){
    case 0: //goForward
      if (digitalRead(LS) == LOW && digitalRead(RS) == HIGH && digitalRead(CS) == HIGH) state=1;
      if (digitalRead(LS) == HIGH && digitalRead(RS) == LOW && digitalRead(CS) == HIGH) state=2;    
      if (digitalRead(LS) == LOW && digitalRead(RS) == HIGH && digitalRead(CS) == LOW)  state=3;
      if (digitalRead(LS) == HIGH && digitalRead(RS) == LOW && digitalRead(CS) == LOW)  state=4;
      break;
  
    case 1: //goAheadRight
      if (digitalRead(RS) == LOW && digitalRead(CS) == HIGH)  state=0; 
      if (digitalRead(RS) == HIGH && digitalRead(CS) == LOW)  state=3;
      break;

    case 2: //goAheadLeft
      if (digitalRead(LS) == LOW  && digitalRead(CS) == HIGH)  state=0;    
      if (digitalRead(LS) == HIGH && digitalRead(CS) == LOW)   state=4;
      break;

    case 3: //goHardRight
      if (digitalRead(RS) == LOW  && digitalRead(CS) == HIGH)   state=0;    
      if (digitalRead(RS) == HIGH && digitalRead(CS) == HIGH)   state=1;
      break;

    case 4: //goHardLeft
      if (digitalRead(LS) == LOW  && digitalRead(CS) == HIGH)   state=0;    
      if (digitalRead(LS) == HIGH && digitalRead(CS) == HIGH)   state=2;
      break;
  }

  //Take Action
  switch (state){
    case '0'
      goAhead();
      break;

    case '1'
      goAheadRight();
      break;

    case '2'
      goAheadLeft();
      break;

    case '3'
      goHardRight();
      break;      
  }

    case '4'
      goHardLeft();
      break;  
}
  
