#define CS 3 //center sensror
#define LS 7
#define RS 4
#define ENA 5 
#define ENB 6
#define IN_10  10
#define IN_11  11
#define IN_12  12
#define IN_13  13
float circumference = (65.0 * 3.14) / 10.0; // Calculate wheel circumference in cm

////////////declarations of vars ///////////
int photo =0 ;
///////////end//////////
///////////////servo pins ///////
#define ECHOPIN 8
#define TRIGPIN 9
/////////////////////////////////

//////////////photo interruptor pins ////////////
#define speedSensor 3
/////////////////////////////////////////

int command;
float distance;

/////////////normal movement of car /////////////////////

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
     
     //old// analogWrite(ENB, speedCar);

  }

void goLeft(int speedCar){

      digitalWrite(IN_10, LOW);
      digitalWrite(IN_11, LOW);
      analogWrite(ENB, 255);
    //old//  analogWrite(ENA, speedCar);

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
//////////new///////////

void goSteering(int speedRight, int speedLeft){
      
      digitalWrite(IN_10, LOW);
      digitalWrite(IN_11, HIGH);
      analogWrite(ENA, speedLeft);

      digitalWrite(IN_12, LOW);
      digitalWrite(IN_13, HIGH);
      analogWrite(ENB, speedRight);
 
  }
///////////////////
void goBackRight(int speedCar){ 

      digitalWrite(IN_10, HIGH);
      digitalWrite(IN_11, LOW);
      analogWrite(ENA, speedCar);


      digitalWrite(IN_12, HIGH);
      digitalWrite(IN_13, LOW);
      analogWrite(ENB, 0);

  }

void goBackLeft(int speedCar){ 

      digitalWrite(IN_10, HIGH);
      digitalWrite(IN_11, LOW);
      analogWrite(ENA, 0);

      digitalWrite(IN_12, HIGH);
      digitalWrite(IN_13, LOW);
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
void rotateRight(int speedCar){ 

      digitalWrite(IN_10, LOW);
      digitalWrite(IN_11, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_12, HIGH);
      digitalWrite(IN_13, LOW);
      analogWrite(ENB, speedCar);

  }
  void rotateLeft(int speedCar){ 

      digitalWrite(IN_11, LOW);
      digitalWrite(IN_10, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_13, HIGH);
      digitalWrite(IN_12, LOW);
      analogWrite(ENB, speedCar);

  }
void brake(){ 

      digitalWrite(IN_10, LOW);
      digitalWrite(IN_11, LOW);
      analogWrite(ENA, 255);

      digitalWrite(IN_12, LOW);
      digitalWrite(IN_13, LOW);
      analogWrite(ENB, 255);

  }
//////////////////////////////////////////////////////////////



/////////////elcode el2araf bta3 medo bta3 circle w infinity


void circle_old(int leftWheelSpeed , int rightWheelSpeed , float circleRatio)
{
      digitalWrite(IN_10, LOW);
      digitalWrite(IN_11, HIGH);
      analogWrite(ENA, leftWheelSpeed);

      digitalWrite(IN_12, LOW);
      digitalWrite(IN_13, HIGH);
      analogWrite(ENB, rightWheelSpeed);
      
      float diameter = 100;
      
      if (rightWheelSpeed > leftWheelSpeed) diameter += 13.3*2;
      
    //while distance covered by car less than circufrence of a 1 meter radius circle
      while((photo/20)*6.5*3.147 < (diameter*3.147 * circleRatio ))
        {
          tryy();
          //if(Serial.read() == 'S') break;
          //Serial.println(photo);
        }
        
      photo=0;
      stopRobot();
}

void circle(int leftWheelSpeed , int rightWheelSpeed , float circleRatio , float diameter)
{
  photo=0;
      digitalWrite(IN_10, LOW);
      digitalWrite(IN_11, HIGH);
      analogWrite(ENA, leftWheelSpeed);

      digitalWrite(IN_12, LOW);
      digitalWrite(IN_13, HIGH);
      analogWrite(ENB, rightWheelSpeed);
      
      //float diameter = 200;
      
    //while distance covered by car less than circufrence of a 1 meter radius circle
      while((photo/41)*6.5*3.147 < (diameter*3.147 * circleRatio ))
        {
          tryy();
          //if(Serial.read() == 'S') break;
          //Serial.println(photo);
        }
        
      photo=0;
      stopRobot();
}




void infinity(int leftWheelSpeed , int rightWheelSpeed)
{
  circle(leftWheelSpeed , rightWheelSpeed , 1 ,100);
  delay(1000);
  circle(rightWheelSpeed , leftWheelSpeed , 1 ,100);
/*
  circle(leftWheelSpeed , rightWheelSpeed , 0.5 );
  delay(1000);
  goAngle(-45);
  delay(1000);
  goAhead(CmtoSteps(70));
  delay(1000);
  circle(rightWheelSpeed , leftWheelSpeed , 0.5 );
  delay(1000);
  goAngle(45);
  delay(1000);
  goAhead(CmtoSteps(70));
  delay(1000);
  goAngle(45);*/
}



////////////////////////












////////////////////////////////////////////new//accurate//////////////////////////


int CmtoSteps(float cm)
{return (cm*41)/circumference ;}

void goBack (float cm) 
{
  int steps=(cm*41)/circumference ;
 photo =0 ;
 Choosetryy();
while (photo < steps) 
{ tryy();goBack(100);
}
 photo =0 ;
  brake() ;

}
void goAhead (float cm) 
{
  int steps =((cm-5)*41)/circumference ;
   photo=0 ;
   Choosetryy();

int diff ,speedd;
   
while (photo < steps) 
{
  tryy();
 diff =steps-photo ;

 if(steps<70) speedd=100;
else if(diff >70)speedd= 100; //kant 50
// else if(diff >30)speedd= 90;
// else if(diff >20)speedd= 80;
//else if(diff >10)speedd= 70;
else speedd=65 ;
 goAhead((int)speedd);
}
photo =0 ;
  brake() ;
}

void goSteps(int steps){
  photo=0 ;int diff ,speedd;
Choosetryy();
while (photo < steps) 
{tryy();
diff =steps-photo ;
//if(steps<70) speedd=100;
// if(diff >50)speedd= 100;
// else if(diff >30)speedd= 90;
// else if(diff >20)speedd= 80;
//else if(diff >10)speedd= 70;
//else
//if(diff<10) speedd=70 ;
//else
speedd=100 ;

rotateRight(speedd);

}    
photo =0 ; brake() ;
}

  /////////////////////////7ngarrb ennaha tlef bdayra qotr so8ayyar 3shan elzawya///////
  
void goSteps2(int steps, int speedd){
  photo=0 ;
Choosetryy();
while (photo < steps) 
{tryy();
goLeft(speedd);
}    
photo =0 ; brake() ;
}
/////////////////////end/////////////////

void goAngle (float th) 
{
//int steps = (th*131)/360 ; //kant 65
if (th==90) goSteps(32);
else if(th==180) goSteps(74);
else { int x =((th*145)/360);goSteps(x);  }
}

float Calcvel (){
unsigned long start  ,endd;
photo =0 ; 
Choosetryy();
while (photo<42){
  tryy();
  if (photo==1)  { start= millis(); }
 
  if (photo==42)  { endd= millis(); }

  }
 
   float vel = (((float)(endd-start))/1000.0);

return((circumference)/vel);
  }
////////////////those two fns for counting pulses in photointerruptor ////////
bool flag;
void Choosetryy(){ if(digitalRead(speedSensor)) flag=LOW ; else flag=HIGH ;}

void tryy (){
 
if (digitalRead(speedSensor) && flag )      {photo++ ;flag = LOW ;}
if (!digitalRead(speedSensor)&& !flag)      {photo++ ;flag =HIGH ;}

}

//////////////////////end /////////////////////////////

///////////////////el functions bta3t el shapes elneha2y/////////////
void square(){
  goAhead((float)100.0);
  delay(500);
  goSteps(30);
  delay(500);
  goAhead((float)100.0);
  delay(500);
  goSteps(30);
  delay(500);goAhead((float)100.0);
  delay(500);
  goSteps(30);
  delay(500);goAhead((float)100.0);
  delay(500);
  goSteps(30);
  delay(500);
  }

/////////////////////////////////////////////////////////////
  
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


  


int state =0 ;
  
void setup() {  
    pinMode (speedSensor,INPUT_PULLUP);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
  
    pinMode(IN_10, OUTPUT);
    pinMode(IN_11, OUTPUT);
    pinMode(IN_12, OUTPUT);
    pinMode(IN_13, OUTPUT);

    pinMode(ECHOPIN, INPUT);
    pinMode(TRIGPIN, OUTPUT);
/*/
    pinMode(LS, INPUT);
    pinMode(RS, INPUT);
/*/

  pinMode(LS , INPUT);
  pinMode(RS , INPUT);
  pinMode(CS , INPUT);
    
    
    Serial.begin(9600); 

  } 

  
  int cd; 
  float dd;
  String ab ;
int stateold =0;
int flagg = 0;
void loop(){
 

if (Serial.available()>0) {
  command = Serial.read();

  stopRobot();

  
switch (command) {
case 'F':goAhead((int)Serial.readString().toInt());break;
case 'B':goBack((int)Serial.readString().toInt());break;
case 'L':goLeft((int)Serial.readString().toInt());break;
case 'R':goRight((int)Serial.readString().toInt());break;
case 'I':goAheadRight((int)Serial.readString().toInt());break;
case 'G':goAheadLeft((int)Serial.readString().toInt());break;
case 'J':goBackRight((int)Serial.readString().toInt());break;
case 'H':goBackLeft((int)Serial.readString().toInt());break;
case 'D':goAhead((float)Serial.readString().toFloat()); break;
case 'o' : goBack((float)photo); break ; 
 case'A':goAngle((float)Serial.readString().toFloat());break;
 case 'P' :goSteps( Serial.readString().toInt()); break;
 case'q': square();break;
 ////////////////7otto ento sor3a w edooha steps w shoofo anhy rakam elly 7ygeeb 90 w anhy elly 7ygeeb 45 w 30 w 180 w shoofo iza thabteen walla la ...garrabo t8ayyaro fl sor3a shwayya w shoofo
 case 'T':goSteps2(Serial.readString().toInt(),155);
 //////////////////////////////////
 case 'c':
/////////////code bta3 rf3t
while(1){stateold=state;
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
 
if(stateold!=state) {stopRobot();delay(50);}
  
  switch (state){
    case 0:{
      goAhead(95);
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
      goAhead(95);
      break; } 
  }

  

  if (Serial.read()=='s') break ;
  }
////////////////////////////
break;

}
}

}
