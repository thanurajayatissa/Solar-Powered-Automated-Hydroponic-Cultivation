#define trig 4
#define echo 2
#define RELAY 8

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);


String WL="Water level:";

void setup() {

  lcd.clear();
  lcd.init();                      
  lcd.init();
  lcd.backlight();
   
  pinMode(trig, OUTPUT);

  pinMode(echo,INPUT);

   pinMode(RELAY, OUTPUT);

  Serial.begin(9600);

}
void loop() {

  digitalWrite(trig,LOW);

  delay(1);

  digitalWrite(trig,HIGH);

  delay(1);

  digitalWrite(trig,LOW);



  long t = pulseIn(echo,HIGH);



  

  long cm = t / 29 / 2;


delay(3000);

  lcd.clear();
     lcd.setCursor(0,0);
     lcd.print(WL + (45- cm)); //Depth of the water tank is taken as 45 
     lcd.print("cm");

     delay(3000);
  
  Serial.print(WL + (45- cm));
  Serial.println("cm");

    
  delay(1000);

   if (cm > 30) {            
    digitalWrite(RELAY, HIGH);
    delay(2000);
  }
  else {
    digitalWrite(RELAY, LOW);
    delay(2000);
  }
  delay(3000);
}
