#define leftMostSensor 2
#define leftSensor 3
#define centerSensor 4
#define rightSensor 5
#define rightMostSensor 6
#define leftMotor A0
#define rightMotor A1

int state = 0;
/*
 * 0 MoveForward
 * 1 TurnRight
 * 2 TurnLeft
 * 3 HardTurnLeft
 * 4 HardTurnRight
 */

void setup() 
{
  
  pinMode(leftMostSensor , INPUT);
  pinMode(leftSensor , INPUT);
  pinMode(centerSensor , INPUT);
  pinMode(rightSensor , INPUT);
  pinMode(rightMostSensor , INPUT);

  pinMode(leftMotor , OUTPUT);
  pinMode(rightMotor , OUTPUT);

}

void loop() {

  switch (state)
  {
    case 0:
      if (digitalRead(leftSensor) == 1)
        state = 2;
      if (digitalRead(rightSensor) == 1)
        state = 1;
      break;
        
    case 1:
      if (digitalRead(centerSensor) == 1)
        state = 0;
      if (digitalRead(leftSensor) == 1)
        state = 2;
      if (digitalRead(rightMostSensor) == 1)
        state = 3;
       break;
       
     case 2:
      if (digitalRead(centerSensor) == 1)
        state = 0;
      if (digitalRead(rightSensor) == 1)
        state = 1;
      if (digitalRead(leftMostSensor) == 1)
        state = 4;
      break;
        
      case 3:
       if (digitalRead(centerSensor) == 1)
         state = 0;
       if (digitalRead(rightSensor) == 1)
         state = 1;
       break;

        case 4:
          if (digitalRead(centerSensor) == 1)
            state = 0;
          if (digitalRead(leftSensor) == 1)
            state = 2;
          break;
  }
    switch (state)
  {
    case 0:
      digitalWrite(leftMotor , 1);
      digitalWrite(rightMotor , 1);
      break;

     case 1:
      digitalWrite(leftMotor , 1);
      digitalWrite(rightMotor , 0);
      break;
      
    case 2:
      digitalWrite(leftMotor , 0);
      digitalWrite(rightMotor , 1);
      break;
      

  }
}
