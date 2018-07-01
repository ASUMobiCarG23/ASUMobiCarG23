void setup() {
  // put your setup code here, to run once:
pinMode ( 12 , OUTPUT ) ;
pinMode ( 13 , OUTPUT ) ;

}
int state =0 ;
void loop() {


if ( digitalRead (LS )== HIGH && digitalRead (CS)== LOW ) 
  {
  state=1 ;
  
  
  }
  if ( digitalRead (LS )== LOW && digitalRead (CS)== HIGH ) 
  {
  state =2 ;
  
  
  }

  if ( digitalRead (LS )== HIGH && digitalRead (CS)== HIGH ) 
  {
  
  
  
  }


  if ( digitalRead (LS)== LOW && digitalRead (CS)== LOW ) 
  {
  
  state =0 ;
  
  }

  if (state==0) gostraight ;
    if (state==1) goleftslow ;
  if (state==2) gorightslow ;


}
