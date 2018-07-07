int dir_input=5;
int dir_state;
int turn_1=13;
int turn_2=12;
int turn_A=9;
int turn_B=8;
int speed_in=A0;
float speed_state;
int speed_1=11;
int speed_2=10;
void setup(){
  
  pinMode(dir_input,INPUT);
  pinMode(turn_1,OUTPUT);
  pinMode(turn_2,OUTPUT);
   pinMode(turn_A,OUTPUT);
  pinMode(turn_B,OUTPUT);
  
  
  }
  void loop(){
    dir_state=digitalRead(dir_input);
    if(dir_state==HIGH){
      digitalWrite(turn_1,HIGH);
            digitalWrite(turn_2,LOW);
digitalWrite(turn_A,HIGH);
            digitalWrite(turn_B,LOW);
      }
    else{
       digitalWrite(turn_1,LOW);
            digitalWrite(turn_2,HIGH);
      
            digitalWrite(turn_A,LOW);
            digitalWrite(turn_B,HIGH);
      
      }
     speed_state= (analogRead(speed_in))/4;
     analogWrite(speed_1,speed_state);
     
         analogWrite(speed_2,speed_state);

    }
