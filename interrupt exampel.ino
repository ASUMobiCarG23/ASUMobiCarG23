//int led2 12;
//int temp 10;
void setup()
{
  pinMode(13, OUTPUT) ;
  pinMode(12, OUTPUT) ;
  pinMode(2, INPUT) ;
  attachInterrupt(0, tempsens, FALLING);
}
void tempsens()
{
    if (digitalRead(2) == HIGH)
        digitalWrite(12 , HIGH);
    else 
       digitalWrite(12 , LOW);     
}

void loop()
{
  interrupts();
 digitalWrite(13, HIGH );   
  delay(100);           
 digitalWrite(13, LOW);   
  delay(100);  

}
