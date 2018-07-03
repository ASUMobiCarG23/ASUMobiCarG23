#define ENA 5 
#define ENB 6

#define IN_10  10
#define IN_11  11

#define IN_12  12
#define IN_13  13

#define ECHOPIN 8
#define TRIGPIN 9


int command;
int speedCar = 100;
float distance;

// Constant for steps in disk
const float stepcount = 20.00;  // 20 Slots in disk, change if different
 
// Constant for wheel diameter
const float wheeldiameter = 65; // Wheel diameter in millimeters, change if different
 
// Integers for pulse counters
volatile int counter_A = 0;

void ISR_countA()  
{
  counter_A++;  // increment Motor A counter value
} 





// Function to convert from centimeters to steps
int CMtoSteps(float cm) {
 
  int result;  // Final calculation result
  float circumference = (wheeldiameter * 3.14) / 10; // Calculate wheel circumference in cm
  float cm_step = circumference / stepcount;  // CM per Step
  
  float f_result = cm / cm_step;  // Calculate result as a float
  result = (int) f_result; // Convert to an integer (note this is NOT rounded)
  
  return result;  // End and return result
 
}
 

void setup() {  
    
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
  
    pinMode(IN_10, OUTPUT);
    pinMode(IN_11, OUTPUT);
    pinMode(IN_12, OUTPUT);
    pinMode(IN_13, OUTPUT);

    pinMode(ECHOPIN, INPUT);
    pinMode(TRIGPIN, OUTPUT);
// Attach the Interrupts to their ISR's
  attachInterrupt(digitalPinToInterrupt (MOTOR_A), ISR_countA, RISING);  // Increase counter A when speed sensor pin goes High
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

  void goAhead(int steps){ 
     counter_A = 0;  //  reset counter A to zero
  
      digitalWrite(IN_10, LOW);
      digitalWrite(IN_11, HIGH);
     
      digitalWrite(IN_12, LOW);
      digitalWrite(IN_13, HIGH);
     

      ///////////////////////////////////////////////////////////////////////////////////////////////////

       while (steps > counter_A ) {
   
    if (steps > counter_A) {
    analogWrite(ENA, speedCar);
    analogWrite(ENB, speedCar);
    } else {
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    }
       }
    
  // Stop when done
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  counter_A = 0;  //  reset counter A to zero
   
 
}




  }

  void goBack(){ 

      digitalWrite(IN_10, HIGH);
      digitalWrite(IN_11, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_12, HIGH);
      digitalWrite(IN_13, LOW);
      analogWrite(ENB, speedCar);
  }

void goBack(int steps){ 

      counter_A = 0;  //  reset counter A to zero
      digitalWrite(IN_10, HIGH);
      digitalWrite(IN_11, LOW);
     
      digitalWrite(IN_12, HIGH);
      digitalWrite(IN_13, LOW);
      

      //////////////////////////////////////////////////////////////////

         while (steps > counter_A ) {
   
    if (steps > counter_A) {
    analogWrite(ENA, speedCar);
    analogWrite(ENB, speedCar);
    } else {
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    }
       }
    
  // Stop when done
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  counter_A = 0;  //  reset counter A to zero
   
 

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
  
void loop(){


if (Serial.available() > 0) {
  command = Serial.read();
  
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
case '6':speedCar =190;break;
case '7':speedCar = 205;break;
case '8':speedCar = 220;break;
case '9':speedCar = 235;break;
case 'q':speedCar = 255;break;
case 'D':
distance*=10;
distance=(serial.read()-'0');
goahead(CmtoSteps(distance));
delay(10);
break;
case 'S':
distance*=10;
distance=(serial.read()-'0');
delay(10);
goaback(CmtoSteps(distance));
break;
}
}

}
