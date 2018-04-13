#include<Servo.h>
Servo myservo;
/* * Created by Vasilakis Michalis // 12-12-2014 ver.1
 * Project: Control RC Car via Bluetooth with Android Smartphone
 * http://www.ardumotive.com/arduino-car
 * More information at www.ardumotive.com
 */
 
//L293 Connection   
//elarqam ely ana msaweha deh arqam elpins fel arduino
  const int motorURF  = 5;  // Pin  2 of L293    omar upper right forwad 
  const int motorURB = 6;  // Pin  7 of L293    omar upper right backward
  const int motorULF  = 10; // Pin 10 of L293    omar upper left forward
  const int motorULB = 9;  // Pin 14 of L293   omar upper left backward
   //zawdt 0 bs 3n ely fo2
 /* const int motorLRF  = 5;  // Pin  2 of L293    omar lower right forward
  const int motorLRB = 6;  // Pin  7 of L293    omar lower right backward
  const int motorLLF  = 10; // Pin 10 of L293    omar lower left forward
  const int motorLLB = 9;  // Pin 14 of L293   omar lower left backward*/
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
  long previousMillis = -1000*10;// -1000*10=-10sec. to read the first value. If you use 0 then you will take the first value after 10sec.  
  long interval = 1000*10;       // interval at which to read battery voltage, change it if you want! (10*1000=10sec)
  unsigned long currentMillis;   //unsigned long currentMillis;
//Useful Variables
  int i=0;
  int j=0;
  int state;
  int vSpeed=200;     // Default speed, from 0 to 255

void setup() {
    // Set pins as outputs:
    myservo.attach(11);
    pinMode(motorURF, OUTPUT);
    pinMode(motorURB, OUTPUT);
    pinMode(motorULF, OUTPUT);
    pinMode(motorULB, OUTPUT);
  /*  pinMode(motorLRF, OUTPUT);
    pinMode(motorLRB, OUTPUT);
    pinMode(motorLLF, OUTPUT);
    pinMode(motorLLB, OUTPUT);*/
    pinMode(lights, OUTPUT); 
    pinMode(BTState, INPUT);    
    // Initialize serial communication at 9600 bits per second:
    Serial.begin(9600);
}
 
void loop() {
  //Stop car when connection lost or bluetooth disconnected
     myservo.write(90);
     if(digitalRead(BTState)==LOW) { state='S'; }

  //Save income data to variable 'state'
    if(Serial.available() > 0){     
      state = Serial.read();   
    }
  
  //Change speed if state is equal from 0 to 4. Values must be from 0 to 255 (PWM)
    if (state == '0'){
      vSpeed=0;}
    else if (state == '1'){
      vSpeed=100;}
    else if (state == '2'){
      vSpeed=180;}
    else if (state == '3'){
      vSpeed=200;}
    else if (state == '4'){
      vSpeed=255;}
 	  
  /***********************Forward****************************/
  //If state is equal with letter 'F', car will go forward!
    if (state == 'F') {
    	analogWrite(motorURB,vSpeed);   analogWrite(motorULB,vSpeed);
        /*analogWrite(motorLRF, vSpeed);      analogWrite(motorLLF, vSpeed);*/ 
    }
  /**********************Forward Left************************/
  //If state is equal with letter 'G', car will go forward left
    else if (state == 'G') {
       myservo.write(45);
    	  
        analogWrite(motorURB,0);    
        analogWrite(motorULB,vSpeed);
    }
  /**********************Forward Right************************/
  //If state is equal with letter 'I', car will go forward right
    else if (state == 'I') {
      
      	myservo.write(135);
        analogWrite(motorULB,0);  
        analogWrite(motorURB,vSpeed);    
    }
  /***********************Backward****************************/
  //If state is equal with letter 'B', car will go backward
    else if (state == 'B') {
    	 myservo.write(90);
      analogWrite(motorURF, vSpeed); analogWrite(motorULF, vSpeed);
        //analogWrite(motorLLB,vSpeed);  // analogWrite(motorLRB,vSpeed); 
    }
  /**********************Backward Left************************/
  //If state is equal with letter 'H', car will go backward left
    else if (state == 'H') {
      myservo.write(135);
    	analogWrite(motorURF, vSpeed);   
        analogWrite(motorULF, 0);   
    }
  /**********************Backward Right************************/
  //If state is equal with letter 'J', car will go backward right
    else if (state == 'J') {
    	myservo.write(45);
        analogWrite(motorURF, 0);  
        analogWrite(motorULF, vSpeed);     
    }
  /***************************Left*****************************/
  //If state is equal with letter 'L', wheels will turn left
    else if (state == 'L') {
      myservo.write(0);
    	analogWrite(motorURF, vSpeed);  
       analogWrite(motorULB,vSpeed); 
    }
  /***************************Right*****************************/
  //If state is equal with letter 'R', wheels will turn right
    else if (state == 'R') {
      myservo.write(180);
    	analogWrite(motorURB,vSpeed);   //analogWrite(motorLRB,vSpeed); 
        //analogWrite(motorLLF, vSpeed); 
        analogWrite(motorULF, vSpeed); 		
    }
  /************************Lights*****************************/
  //If state is equal with letter 'W', turn leds on or of off
   /* else if (state == 'W') {
      if (i==0){  
         digitalWrite(lights, HIGH); 
         i=1;
      }
      else if (i==1){
         digitalWrite(lights, LOW); 
         i=0;
      }
      state='n';
    }*/
  /**********************Horn sound***************************/
  //If state is equal with letter 'V', play (or stop) horn sound
    /*else if (state == 'V'){
      if (j==0){  
         tone(buzzer, 1000);//Speaker on 
         j=1;
      }
      else if (j==1){
         noTone(buzzer);    //Speaker off 
         j=0;
      }
      state='n';  
    }*/
  /************************Stop*****************************/
  //If state is equal with letter 'S', stop the car
    else if (state == 'S'){
      myservo.write(90);
        analogWrite(motorURF, 0);  analogWrite(motorURB, 0); 
        analogWrite(motorULF, 0);  analogWrite(motorULB, 0);
       /* analogWrite(motorLRF, 0);  analogWrite(motorLRB, 0); 
        analogWrite(motorLLF, 0);  analogWrite(motorLLB, 0);*/
    }
  /***********************Battery*****************************/
  //Read battery voltage every 10sec.
    currentMillis = millis();
    if(currentMillis - (previousMillis) > (interval)) {
       previousMillis = currentMillis; 
       //Read voltage from analog pin A0 and make calibration:
       voltage = (analogRead(A0)*5.015 / 1024.0)*11.132;
       //Calculate percentage...ez
       perVolt = (voltage*100)/ maxBattery;
       if      (perVolt<=75)               { level=0; }
       else if (perVolt>75 && perVolt<=80) { level=1; }    //        Battery level
       else if (perVolt>80 && perVolt<=85) { level=2; }    //Min ------------------------   Max
       else if (perVolt>85 && perVolt<=90) { level=3; }    //    | 0 | 1 | 2 | 3 | 4 | 5 | >
       else if (perVolt>90 && perVolt<=95) { level=4; }    //    ------------------------
       else if (perVolt>95)                { level=5; }   
       Serial.println(level);    
    }
    
}
/* this is before line tracking code feature */


