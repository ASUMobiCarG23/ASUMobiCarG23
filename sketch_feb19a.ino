#include<SoftwareSerial.h>

#define TxD 3
#define RxD 2

SoftwareSerial bluetoothSerial(TxD, RxD);
int state;


void setup() {
  // put your setup code here, to run once:

   bluetoothSerial.begin(9600);
  Serial.begin(9600);

pinMode(9,OUTPUT);    //enable 1 controled pwm
pinMode(7,OUTPUT);     //input 1
pinMode(8,OUTPUT);     //input2

pinMode(5,OUTPUT);     //enable 2 controled pwm
pinMode(4,OUTPUT);     // input 1
pinMode(13,OUTPUT);     // input 2





}

void loop() {
  // put your main code here, to run repeatedly:
if(bluetoothSerial.available()>0)
  {
    state=bluetoothSerial.read();
    
    }
    if(state=='F')
    {
      analogWrite(5,255);
      analogWrite(9,255);
      
      digitalWrite(7,HIGH);
      digitalWrite(4,HIGH);

      digitalWrite(8,LOW);
      digitalWrite(13,LOW);

      
      }
else if(state=='B')
{
     analogWrite(5,255);
      analogWrite(9,255);
      
      digitalWrite(7,LOW);
      digitalWrite(4,LOW);

      digitalWrite(8,HIGH);
      digitalWrite(13,HIGH);


  }
else if(state=='S')
{
      analogWrite(5,0);
      analogWrite(9,0);
      
  }
  else if(state=='L')
  {
      analogWrite(5,255);
      analogWrite(9,0);
      
      digitalWrite(7,LOW);
      digitalWrite(4,HIGH);

      digitalWrite(8,LOW);
      digitalWrite(13,LOW);
    
    }
     else if(state=='R')
  { 
      analogWrite(5,0);
      analogWrite(9,255);
      
      digitalWrite(7,HIGH);
      
      digitalWrite(8,LOW);
      
    
  }
  else{
     analogWrite(5,0);
      analogWrite(9,0);
    
    }
 
}
}
