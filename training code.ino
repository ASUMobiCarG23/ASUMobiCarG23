#include<SoftwareSerial.h>

#define TxD 3
#define RxD 2
#define LED_PIN 13

SoftwareSerial bluetoothSerial(TxD, RxD);

char c;

void setup() {
  bluetoothSerial.begin(9600);
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  }

void loop() {
  if(bluetoothSerial.available()){
    c = bluetoothSerial.read();
    Serial.println(c);
    if(c=='1'){
      digitalWrite(LED_PIN, HIGH);          
    }
    if(c=='0'){
      digitalWrite(LED_PIN, LOW);
    }
  }
}
