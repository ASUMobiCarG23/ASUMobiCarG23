#include<SoftwareSerial.h>

#define TxD 3
#define RxD 2

SoftwareSerial bluetoothSerial(TxD, RxD);
int state;

void setup() {
  // put your setup code here, to run once:
   bluetoothSerial.begin(9600);
  Serial.begin(9600);

pinMode(12,OUTPUT);
pinMode(13,OUTPUT);  
pinMode(7,OUTPUT);
pinMode(8,OUTPUT);


pinMode(9,OUTPUT);
pinMode(10,OUTPUT);  
pinMode(A2,OUTPUT);
pinMode(A3,OUTPUT);
pinMode(A4,OUTPUT);
pinMode(A5,OUTPUT);

analogWrite(A0,0);
analogWrite(A1,0);

analogWrite(6,0);
analogWrite(11,0);


}

void loop() {
  if(bluetoothSerial.available()>0)
  {
    state=bluetoothSerial.read();
    
    }
    if(state=='F')
    {
analogWrite(A0,255);
analogWrite(A1,255);
analogWrite(6,255);
analogWrite(11,255);


 analogWrite(A2,255);
      analogWrite(A3,0);
      
      analogWrite(A4,255);
     
analogWrite(A5,0);


      
      analogWrite(9,255);
      analogWrite(10,0);
      
     
analogWrite(5,255);

digitalWrite(4,LOW);

digitalWrite(12,HIGH);
 digitalWrite(13,LOW);
 
digitalWrite(8,HIGH);
 digitalWrite(7,LOW);

 


      
      }
else if(state=='B')
{

analogWrite(A2,0);
      analogWrite(A3,255);
      analogWrite(A4,0);
     
analogWrite(A5,255);

  
    analogWrite(9,0);
      analogWrite(10,255);
     
analogWrite(5,0);

digitalWrite(4,HIGH);
 
 digitalWrite(12,LOW);
 digitalWrite(13,HIGH);

 digitalWrite(8,LOW);
 digitalWrite(7,HIGH);


 analogWrite(A0,255);
analogWrite(A1,255);

analogWrite(6,255);
analogWrite(11,255);

  }
else if(state=='S')
{
  analogWrite(6,0);
  analogWrite(A0,0);
analogWrite(A1,0);
  analogWrite(11,0)
;  
  }
  else if(state=='L')
  {
    analogWrite(A1,0);
  analogWrite(11,0)
;  
     analogWrite(6,255);
     analogWrite(A0,255);


     analogWrite(A2,255);
      analogWrite(A3,0);

      digitalWrite(12,HIGH);
 digitalWrite(13,LOW);
 
digitalWrite(8,HIGH);
 digitalWrite(7,LOW);
    
    
    }
     else if(state=='R')
  { 
    analogWrite(A1,255);
     analogWrite(11,255);
     analogWrite(6,0);
     analogWrite(A0,0);




 analogWrite(A4,255);
     
analogWrite(A5,0);


      
      analogWrite(9,255);
      analogWrite(10,0);
      
     
analogWrite(5,255);

digitalWrite(4,LOW);
  }
 
}
