#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define RELAY 8

LiquidCrystal_I2C lcd(0x27,16,2);  
int pHSense = A0;
int samples = 10;
float adc_resolution = 1024.0;

void setup()
{
  Serial.begin(9600);
  delay(100);
  
    lcd.init();                      
  lcd.init();
  
  lcd.backlight();
  pinMode(RELAY, OUTPUT);
}

float ph (float voltage) {
  return 7 + ((2.5 - voltage) / 0.18);
}

void loop () {
  int measurings=0;
  for (int i = 0; i < samples; i++)
  {
    measurings += analogRead(pHSense);
    delay(10);
  }
    float voltage = 5 / adc_resolution * measurings/samples;
    Serial.print("pH= ");
    Serial.println(ph(voltage));
    delay(3000);
    lcd.clear();
    lcd.print("pH= ");
    lcd.print(ph(voltage));
    delay(3000);

    if (ph(voltage) > 6) {         //pH value 6 is taken as a random value
    digitalWrite(RELAY, HIGH);
    delay(100);
  }
  else {
    digitalWrite(RELAY, LOW);
  }
}
