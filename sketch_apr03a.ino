#include <Servo.h>

#define ENA 5 
#define ENB 6

#define IN_10  10
#define IN_11  11

#define IN_12  12
#define IN_13  13

#define ECHOPIN 8
#define TRIGPIN 9

Servo myservo;
int command;
int speedCar = 100;
int distanceR , distanceL ;
void setup() {

    myservo.attach(3);  
    myservo.write(90);
    
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
  
    pinMode(IN_10, OUTPUT);
    pinMode(IN_11, OUTPUT);
    pinMode(IN_12, OUTPUT);
    pinMode(IN_13, OUTPUT);

    pinMode(ECHOPIN, INPUT);
    pinMode(TRIGPIN, OUTPUT);

  Serial.begin(9600); 

  } 

void goAhead(){ 

      digitalWrite(IN_10, LOW);
      digitalWrite(IN_11, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_12, LOW);
      digitalWrite(IN_13, HIGH);
      analogWrite(ENB, speedCar);

  }

void goBack(){ 

      digitalWrite(IN_10, HIGH);
      digitalWrite(IN_11, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_12, HIGH);
      digitalWrite(IN_13, LOW);
      analogWrite(ENB, speedCar);

  }

void goRight(){ 

      digitalWrite(IN_10, LOW);
      digitalWrite(IN_11, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_12, LOW);
      digitalWrite(IN_13, LOW);
      analogWrite(ENB, speedCar);

  }

void goLeft(){

      digitalWrite(IN_10, LOW);
      digitalWrite(IN_11, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_12, LOW);
      digitalWrite(IN_13, HIGH);
      analogWrite(ENB, speedCar);
        
  }

void goAheadRight(){
      
      digitalWrite(IN_10, LOW);
      digitalWrite(IN_11, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_12, LOW);
      digitalWrite(IN_13, HIGH);
      analogWrite(ENB, speedCar/4);
 
  }

void goAheadLeft(){
      
      digitalWrite(IN_10, LOW);
      digitalWrite(IN_11, HIGH);
      analogWrite(ENA, speedCar/4);

      digitalWrite(IN_12, LOW);
      digitalWrite(IN_13, HIGH);
      analogWrite(ENB, speedCar);
 
  }

void goBackRight(){ 

      digitalWrite(IN_10, HIGH);
      digitalWrite(IN_11, LOW);
      analogWrite(ENA, speedCar);


      digitalWrite(IN_12, HIGH);
      digitalWrite(IN_13, LOW);
      analogWrite(ENB, speedCar/4);

  }

void goBackLeft(){ 

      digitalWrite(IN_10, HIGH);
      digitalWrite(IN_11, LOW);
      analogWrite(ENA, speedCar/4);

      digitalWrite(IN_12, HIGH);
      digitalWrite(IN_13, LOW);
      analogWrite(ENB, speedCar);

  }

void stopRobot(){  

      digitalWrite(IN_10, LOW);
      digitalWrite(IN_11, LOW);
      analogWrite(ENA, speedCar);
      
      digitalWrite(IN_12, LOW);
      digitalWrite(IN_13, LOW);
      analogWrite(ENB, speedCar);
  
  }
 
int lookRight()
{
    myservo.write(10); 
    delay(500);
    float distance = pulseIn(ECHOPIN, HIGH);
    distance = distance/58;
    delay(100);
    myservo.write(90); 
    return distance;
}

int lookLeft()
{
    myservo.write(170); 
    delay(500);
    float distance = pulseIn(ECHOPIN, HIGH);
    distance = distance/58;
    delay(100);
    myservo.write(90); 
    return distance;
}
  
void loop(){

digitalWrite(TRIGPIN, LOW);
delayMicroseconds(2);
digitalWrite(TRIGPIN, HIGH);
delayMicroseconds(10);
digitalWrite(TRIGPIN, LOW);

float distance = pulseIn(ECHOPIN, HIGH);
distance = distance/58;

stopRobot();

if (Serial.available() > 0) {
  command = Serial.read();
  }
  stopRobot();

switch (command) {
case 'F':goAhead();break;
case 'B':goBack();break;
case 'L':goLeft();break;
case 'R':goRight();break;
case 'I':goAheadRight();break;
case 'G':goAheadLeft();break;
case 'J':goBackRight();break;
case 'H':goBackLeft();break;
case '0':speedCar = 100;break;
case '1':speedCar = 115;break;
case '2':speedCar = 130;break;
case '3':speedCar = 145;break;
case '4':speedCar = 160;break;
case '5':speedCar = 175;break;
case '6':speedCar = 190;break;
case '7':speedCar = 205;break;
case '8':speedCar = 220;break;
case '9':speedCar = 235;break;
case 'q':speedCar = 255;break;
}


if(distance < 30){
  stopRobot();
  goBack();
  delay(50);
  stopRobot();
  delay(200);
  distanceR = lookRight();
  distanceL = lookLeft();

  if(distanceR >= distanceL)
  {
    goRight();
    delay(600);
    stopRobot();
  }else
  {
    goLeft();
    delay(600);
    stopRobot();
  }
    
}
}
