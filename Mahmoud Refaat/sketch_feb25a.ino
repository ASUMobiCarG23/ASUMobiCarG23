char t;
#define ECHOPIN 8
#define TRIGPIN 9

void setup() {
pinMode(ECHOPIN, INPUT);
pinMode(TRIGPIN, OUTPUT);
pinMode(2,OUTPUT);
digitalWrite(2,HIGH);
pinMode(13,OUTPUT);
pinMode(12,OUTPUT);
pinMode(11,OUTPUT);
pinMode(10,OUTPUT);

Serial.begin(9600);

}

void loop() {  
digitalWrite(2,HIGH);
if(Serial.available()){
  t = Serial.read();
  Serial.println(t);
}

digitalWrite(TRIGPIN, LOW);
delayMicroseconds(2);
digitalWrite(TRIGPIN, HIGH);
delayMicroseconds(10);
digitalWrite(TRIGPIN, LOW);

float distance = pulseIn(ECHOPIN, HIGH);
distance = distance/58;

Serial.print(distance);
Serial.println(" cm");
delay(50);

if(distance <= 50) {
digitalWrite(13,LOW);
digitalWrite(12,LOW);
digitalWrite(11,LOW);
digitalWrite(10,LOW);  
}
else {
if(t == 'F'){
  digitalWrite(13,HIGH);
  digitalWrite(12,LOW);
  digitalWrite(11,HIGH);
  digitalWrite(10,LOW);
}

else if(t == 'B'){
  digitalWrite(13,LOW);
  digitalWrite(12,HIGH);
  digitalWrite(11,LOW);
  digitalWrite(10,HIGH);
}

else if(t == 'R'){
  digitalWrite(13,LOW);
  digitalWrite(12,LOW);
  digitalWrite(11,HIGH);
  digitalWrite(10,LOW);
}

else if(t == 'L'){
  digitalWrite(13,HIGH);
  digitalWrite(12,LOW);
  digitalWrite(11,LOW);
  digitalWrite(10,LOW);
}
 
else if(t == 'S'){
  digitalWrite(13,LOW);
  digitalWrite(12,LOW);
  digitalWrite(11,LOW);
  digitalWrite(10,LOW);
}
}
delay(100);

}
