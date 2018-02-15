#include <Servo.h>
#include "Ultrasonic.h"
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
int x = 0;
int pos = 0;    // variable to store the servo position
Ultrasonic ultrasonic(11, 10); //(Trig,Echo)
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
/* * Created by Vasilakis Michalis // 12-12-2014 ver.1
   Project: Control RC Car via Bluetooth with Android Smartphone
   http://www.ardumotive.com/arduino-car
   More information at www.ardumotive.com
*/
int da = A0;//pin connected to sound sensor
int threshold = 50; //Set minimum threshold for LED lit
int sensorvalue = 0;

//L293 Connection
//elarqam ely ana msaweha deh arqam elpins fel arduino
const int M1F = 5;  // Pin  2 of L293    omar upper right forwad
const int M1B = 6; // Pin  7 of L293    omar upper right backward
const int M2F  = 9; // Pin 10 of L293    omar upper left forward
const int M2B = 10;  // Pin 14 of L293   omar upper left backward

//Leds connected to Arduino UNO Pin 12
const int lights  = 12;
//Buzzer / Speaker to Arduino UNO Pin 3
const int buzzer = 3 ;
//Bluetooth (HC-06 JY-MCU) State pin on pin 2 of Arduino
const int BTState = 2;
//Calculate Battery Level
const float maxBattery = 9.0;// Change value to your max battery voltage level!
int perVolt;                 // Percentage variable
float voltage = 0.0;         // Read battery voltage
int level;
// Use it to make a delay... without delay() function!
long previousMillis = -1000 * 10; // -1000*10=-10sec. to read the first value. If you use 0 then you will take the first value after 10sec.
long interval = 1000 * 10;     // interval at which to read battery voltage, change it if you want! (10*1000=10sec)
unsigned long currentMillis;   //unsigned long currentMillis;
//Useful Variables
int i = 0;
int j = 0;
int state;
int vSpeed = 200;   // Default speed, from 0 to 255

void setup() {
  // Set pins as outputs:
  lcd.begin(16, 4);
  myservo.attach(11);
  Serial.begin(9600);
  myservo.write(90);
  pinMode(M1F, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(M2F, OUTPUT);
  pinMode(M2B, OUTPUT);
  pinMode(lights, OUTPUT);
  pinMode(BTState, INPUT);
  // Initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}
void forward(int x) {
  analogWrite(M1F, x);
  analogWrite(M1B, 0);
  analogWrite(M2F, x);
  analogWrite(M2B, 0);
}
void reverse(int x)
{
  analogWrite(M1F, 0);
  analogWrite(M1B, x);
  analogWrite(M2F, 0);
  analogWrite(M2B, x);
}
void Stop() {
  analogWrite(M1F, 0);
  analogWrite(M2B, 0);
  analogWrite(M1B, 0);
  analogWrite(M2B, 0);
}
void right(int x) {
  analogWrite(M1B, x);
  analogWrite(M1F, 0);
  analogWrite(M2B, 0);
  analogWrite(M2F, x);
}
void left(int x) {

  analogWrite(M1F, x);
  analogWrite(M1B, 0);
  analogWrite(M2B, x);
  analogWrite(M2F, 0);
}
void fl(int x) {

  analogWrite(M1F, x);
  analogWrite(M1B, 0);
  analogWrite(M2B, 0);
  analogWrite(M2F, 0);
}
void fr(int x) {

  analogWrite(M1F, 0);
  analogWrite(M1B, 0);
  analogWrite(M2B, 0);
  analogWrite(M2F, x);
}
void bl(int x) {

  analogWrite(M1F, 0);
  analogWrite(M1B, x);
  analogWrite(M2B, 0);
  analogWrite(M2F, 0);
}
void br(int x) {

  analogWrite(M1F, 0);
  analogWrite(M1B, 0);
  analogWrite(M2B, x);
  analogWrite(M2F, 0);
}
void loop() {
  //Stop car when connection lost or bluetooth disconnected
  myservo.write(90);
  if (digitalRead(BTState) == LOW) {
    state = 'S';
  }


  //Save income data to variable 'state'
  if (Serial.available() > 0) {
    state = Serial.read();
    Serial.println(state);
    lcd.setCursor(0,0);
    lcd.print("level: ");
    lcd.print(level);
  }

  //Change speed if state is equal from 0 to 4. Values must be from 0 to 255 (PWM)
  if (state == '0') {
    vSpeed = 0;
  }
  else if (state == '1') {
    vSpeed = 25;
  }
  else if (state == '2') {
    vSpeed = 50;
  }
  else if (state == '3') {
    vSpeed = 75;
  }
  else if (state == '4') {
    vSpeed = 100;
  }
  else if (state == '5') {
    vSpeed = 125;
  }
  else if (state == '6') {
    vSpeed = 150;
  }
  else if (state == '7') {
    vSpeed = 175;
  }
  else if (state == '8') {
    vSpeed = 200;
  }
  else if (state == '9') {
    vSpeed = 225;
  }
  else if (state == 'q') {
    vSpeed = 255;
  }

  /***********************Forward****************************/
  //If state is equal with letter 'F', car will go forward!
  if (state == 'F') {
    forward(vSpeed);
    lcd.setCursor(0,1);
    lcd.print("order: Forward");
  }
  /**********************Forward Left************************/
  //If state is equal with letter 'G', car will go forward left
  else if (state == 'G') {
    fl(vSpeed);
    lcd.setCursor(0,1);
    lcd.print("order: Forward Left");
  }
  /**********************Forward Right************************/
  //If state is equal with letter 'I', car will go forward right
  else if (state == 'I') {

    fr(vSpeed);
    lcd.setCursor(0,1);
    lcd.print("order: Forward Right");
  }
  /***********************Backward****************************/
  //If state is equal with letter 'B', car will go backward
  else if (state == 'B') {
    reverse(vSpeed);
    lcd.setCursor(0,1);
    lcd.print("order: Backward");
  }
  /**********************Backward Left************************/
  //If state is equal with letter 'H', car will go backward left
  else if (state == 'H') {
    bl(vSpeed);
    lcd.setCursor(0,1);
    lcd.print("order: Backward Left");
  }
  /**********************Backward Right************************/
  //If state is equal with letter 'J', car will go backward right
  else if (state == 'J') {
    br(vSpeed);
    lcd.setCursor(0,1);
    lcd.print("order: Backward Right");
  }
  /***************************Left*****************************/
  //If state is equal with letter 'L', wheels will turn left
  else if (state == 'L') {
    left(vSpeed);
    lcd.setCursor(0,1);
    lcd.print("order: Left");
  }
  /***************************Right*****************************/
  //If state is equal with letter 'R', wheels will turn right
  else if (state == 'R') {
    right(vSpeed) ;
    lcd.setCursor(0,1);
    lcd.print("order: Right");
  }
  /************************Lights*****************************/
  //If state is equal with letter 'W', turn leds on or of off
   else if (state == 'W') {lcd.setCursor(0,1);
    lcd.print("ULTRASONIC");
    while(1){
if (ultrasonic.Ranging(CM)<30)
    {Stop();
      
  for (pos = 90; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    if(ultrasonic.Ranging(CM)>30){right(vSpeed);delay(1000);myservo.write(90);x=1;break;}
    }
    if (x==0){
     for (pos = 90; pos >= 0; pos -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    if(ultrasonic.Ranging(CM)>30){left(vSpeed);delay(1000);myservo.write(90);break;}
    }
    }
    }
    else {forward(vSpeed);x=0;}
  //output the distance in cm
  Serial.println(ultrasonic.Ranging(CM)); 
  delay(100);
  if(state=='w'){break;} 
    }
}

  /**********************Horn sound***************************/
  //If state is equal with letter 'V', play (or stop) horn sound
  else if (state == 'V'){
    lcd.setCursor(0,1);
    lcd.print("SOUND");
    while(1){
   sensorvalue = analogRead(da);  //Read the analog value
   if(sensorvalue>threshold ){forward(vSpeed);}
    if(state=='v'){break;}
  }
    }
  /************************Stop*****************************/
  //If state is equal with letter 'S', stop the car
  else if (state == 'S') {
    Stop();
    lcd.setCursor(0,1);
    lcd.print("order: Stop");
  }
  /***********************Battery*****************************/
  //Read battery voltage every 10sec.
  currentMillis = millis();
  if (currentMillis - (previousMillis) > (interval)) {
    previousMillis = currentMillis;
    //Read voltage from analog pin A0 and make calibration:
    voltage = (analogRead(A0) * 5.015 / 1024.0) * 11.132;
    //Calculate percentage...ez
    perVolt = (voltage * 100) / maxBattery;
    if      (perVolt <= 75)               {
      level = 0;
    }
    else if (perVolt > 75 && perVolt <= 80) {
      level = 1;  //        Battery level
    }
    else if (perVolt > 80 && perVolt <= 85) {
      level = 2;  //Min ------------------------   Max
    }
    else if (perVolt > 85 && perVolt <= 90) {
      level = 3;  //    | 0 | 1 | 2 | 3 | 4 | 5 | >
    }
    else if (perVolt > 90 && perVolt <= 95) {
      level = 4;  //    ------------------------
    }
    else if (perVolt > 95)                {
      level = 5;
    }
    //    Serial.println(level);
  }

}



