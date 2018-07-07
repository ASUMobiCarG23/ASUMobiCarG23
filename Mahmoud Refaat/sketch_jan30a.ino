#include <LiquidCrystal.h>
LiquidCrystal lcd (2,3,4,5,6,7) ;

int temp;

void setup() {
pinMode(13,OUTPUT);
lcd.begin(16,2);
for ( int x=0 ; x < 10 ; x++ )
{
  lcd.setCursor(x,0);
  lcd.print(".");
  delay(1000);
  }
  lcd.print("!");
  delay(1000);
  lcd.print("Done");
  delay(1000);
  lcd.clear();
}

void loop() {
temp=map(analogRead(0),0,306,0,150);
lcd.setCursor(0,0);
lcd.print("Temp Measurement");
lcd.setCursor(0,1);
lcd.print("Temp = ");
lcd.print(temp);
lcd.print(" C");

if (temp >= 25)
digitalWrite(13,HIGH);
else
digitalWrite(13,LOW);

delay(1000);
lcd.clear();

}
