#define ENA 5    // ENABLE Motors 1
#define ENB 6    // ENABLE Motors 2

#define IN_10  10   // Left Motor terminal 1 
#define IN_11  11   // Left Motor terminal 2
#define IN_12  12   // Right Motor terminal 1
#define IN_13  13   // Right Motor terminal 2

#define CS 3   // Centre Sensor 
#define LS 7   // Left Sensor 
#define RS 4   // Right Sensor 

#define speedSensor 3

#define ECHOPIN 8
#define TRIGPIN 9

int state =0; 


void goAhead(int speedCar){ 

      digitalWrite(IN_10, LOW);
      digitalWrite(IN_11, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_12, LOW);
      digitalWrite(IN_13, HIGH);
      analogWrite(ENB, speedCar);

  }
  
  void goBack(int speedCar){ 

      digitalWrite(IN_10, HIGH);
      digitalWrite(IN_11, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_12, HIGH);
      digitalWrite(IN_13, LOW);
      analogWrite(ENB, speedCar);
  }


void goRight(int speedCar){ 

      digitalWrite(IN_10, LOW);
      digitalWrite(IN_11, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_12, LOW);
      digitalWrite(IN_13, LOW);
      analogWrite(ENB, 255);
     
   // analogWrite(ENB, speedCar);

  }

void goLeft(int speedCar){

      digitalWrite(IN_10, LOW);
      digitalWrite(IN_11, LOW);
      analogWrite(ENB, 255);
  
   // analogWrite(ENA, speedCar);

      digitalWrite(IN_12, LOW);
      digitalWrite(IN_13, HIGH);
      analogWrite(ENB, speedCar);
        
  }

void goAheadRight(int speedCar){
      
      digitalWrite(IN_10, LOW);
      digitalWrite(IN_11, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_12, LOW);
      digitalWrite(IN_13, HIGH);
      analogWrite(ENB, speedCar/4);
 
  }

void goAheadLeft(int speedCar){
      
      digitalWrite(IN_10, LOW);
      digitalWrite(IN_11, HIGH);
      analogWrite(ENA, speedCar/4);

      digitalWrite(IN_12, LOW);
      digitalWrite(IN_13, HIGH);
      analogWrite(ENB, speedCar);
 
  }

void goHardRight(int speedCar){ 

      digitalWrite(IN_10, LOW);
      digitalWrite(IN_11, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_12, HIGH);
      digitalWrite(IN_13, LOW);
      analogWrite(ENB, speedCar);

  }
  
void goHardLeft(int speedCar){ 

      digitalWrite(IN_11, LOW);
      digitalWrite(IN_10, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_13, HIGH);
      digitalWrite(IN_12, LOW);
      analogWrite(ENB, speedCar);

  }
       
void stopRobot(){  

      digitalWrite(IN_10, LOW);
      digitalWrite(IN_11, LOW);
      analogWrite(ENA, 0);
      
      digitalWrite(IN_12, LOW);
      digitalWrite(IN_13, LOW);
      analogWrite(ENB, 0);
  
  }



void setup() {

  Serial.begin(9600); 

    pinMode(IN_10, OUTPUT);
    pinMode(IN_11, OUTPUT);
    pinMode(IN_12, OUTPUT);
    pinMode(IN_13, OUTPUT);

    pinMode (speedSensor,INPUT_PULLUP);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
  
    pinMode(ECHOPIN, INPUT);
    pinMode(TRIGPIN, OUTPUT);

 // pinMode(LS, INPUT);
 // pinMode(RS, INPUT); 

    pinMode(LS , INPUT);
    pinMode(RS , INPUT);
    pinMode(CS , INPUT);
      
} 
 

void loop() {
  
                                        /* Line Follower Code */
                                      
while(1){
  
   // Cases might happen 
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

    case 5: //Intersection
      if (digitalRead(LS) == HIGH  && digitalRead(RS) == HIGH)   state=0;    
      break;
  }

    //Take Action
  switch (state){
    case 0:{
      goAhead(150);
      delay(100);
      break;}

    case 1:{
      goHardRight(80);
      break;}

    case 2:{
      goHardLeft(80);
      break;}

    case 3:{
      goHardRight(80);
      break; }     

    case 4:{
      goHardLeft(80);
      break; }
    case 5:{
      goAhead(100);
      delay(100);
      break; } 
  }

    // breaking the loop 
  if(Serial.read()=='s')
  break;
}

}
