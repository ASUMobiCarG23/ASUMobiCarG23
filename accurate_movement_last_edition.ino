

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


