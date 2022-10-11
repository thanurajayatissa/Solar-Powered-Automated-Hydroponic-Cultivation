#include <Fuzzy.h>
#include <FuzzyComposition.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzyOutput.h>
#include <FuzzyRule.h>
#include <FuzzyRuleAntecedent.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzySet.h>
#include <DHT.h>
#include <Arduino.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

#define DHTPIN 7
#define DHTTYPE DHT11
#define Valve3 6
#define EFan 3
DHT dht(DHTPIN,DHTTYPE);

Fuzzy *fuzzy = new Fuzzy();

String H = "Hum: ";

void setup() {

  lcd.clear();
  lcd.init();                      
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("HYDROPONIC");
  
  Serial.begin(9600);
  dht.begin();
  pinMode(DHTPIN, INPUT);
  pinMode(Valve3, OUTPUT);
  pinMode(EFan, OUTPUT);
  
  FuzzySet *verysmall5 = new FuzzySet(0, 5, 5, 10);
  FuzzySet *small5 = new FuzzySet(0, 20, 20, 40);
  FuzzySet *mid5 = new FuzzySet(30, 40, 40, 50);
  FuzzySet *big5 = new FuzzySet(40, 60, 60, 80);
  FuzzySet *verybig5 = new FuzzySet(60, 80, 80, 100);

  
 
  FuzzySet *lowb5 = new FuzzySet(0, 50, 50, 100);
  FuzzySet *midb5 = new FuzzySet(50, 100, 100, 150);
  FuzzySet *highb5 = new FuzzySet(100, 150, 150, 200);
  FuzzySet *veryhighb5 = new FuzzySet(150, 200, 200, 255);

  FuzzySet *lowb8 = new FuzzySet(0, 50, 50, 100);
  FuzzySet *midb8 = new FuzzySet(50, 100, 100, 150);
  FuzzySet *highb8 = new FuzzySet(100, 150, 150, 200);
  FuzzySet *veryhighb8 = new FuzzySet(150, 200, 200, 255);

  
  

  FuzzyInput *humidity = new FuzzyInput(1);
  humidity->addFuzzySet(verysmall5);
  humidity->addFuzzySet(small5);
  humidity->addFuzzySet(mid5);
  humidity->addFuzzySet(big5);
  humidity->addFuzzySet(verybig5);
  fuzzy->addFuzzyInput(humidity);

  FuzzyOutput *HumValve = new FuzzyOutput(1);
  HumValve->addFuzzySet(lowb5);
  HumValve->addFuzzySet(midb5);
  HumValve->addFuzzySet(highb5);
  HumValve->addFuzzySet(veryhighb5);
  fuzzy->addFuzzyOutput(HumValve);

  FuzzyOutput *HumFan = new FuzzyOutput(2);
  HumFan->addFuzzySet(lowb8);
  HumFan->addFuzzySet(midb8);
  HumFan->addFuzzySet(highb8);
  HumFan->addFuzzySet(veryhighb8);
  fuzzy->addFuzzyOutput(HumFan);

  

  FuzzyRuleAntecedent *ifhumidityVerySmall5 = new FuzzyRuleAntecedent();
  ifhumidityVerySmall5->joinSingle(verysmall5);
  FuzzyRuleConsequent *thenHumValveVeryHigh5 = new FuzzyRuleConsequent();
  thenHumValveVeryHigh5->addOutput(veryhighb5);
  FuzzyRule *fuzzyRule1 = new FuzzyRule(1, ifhumidityVerySmall5, thenHumValveVeryHigh5);
  fuzzy->addFuzzyRule(fuzzyRule1);

  FuzzyRuleAntecedent *ifhumiditySmall5 = new FuzzyRuleAntecedent();
  ifhumiditySmall5->joinSingle(small5);
  FuzzyRuleConsequent *thenHumValveHigh5 = new FuzzyRuleConsequent();
  thenHumValveHigh5->addOutput(highb5);
  FuzzyRule *fuzzyRule2 = new FuzzyRule(2, ifhumiditySmall5, thenHumValveHigh5);
  fuzzy->addFuzzyRule(fuzzyRule2);

  
  FuzzyRuleAntecedent *ifhumidityMid5= new FuzzyRuleAntecedent();
  ifhumidityMid5->joinSingle(mid5);
  FuzzyRuleConsequent *thenHumValveMid5 = new FuzzyRuleConsequent();
  thenHumValveMid5->addOutput(midb5);
  FuzzyRule *fuzzyRule3 = new FuzzyRule(3, ifhumidityMid5, thenHumValveMid5);
  fuzzy->addFuzzyRule(fuzzyRule3);

  
  FuzzyRuleAntecedent *ifhumidityBig5 = new FuzzyRuleAntecedent();
  ifhumidityBig5->joinSingle(big5);
  FuzzyRuleConsequent *thenHumValveLow5 = new FuzzyRuleConsequent();
  thenHumValveLow5->addOutput(lowb5);
  FuzzyRule* fuzzyRule4 = new FuzzyRule(4, ifhumidityBig5, thenHumValveLow5);
  fuzzy->addFuzzyRule(fuzzyRule4);
  
  
  FuzzyRuleAntecedent *ifhumidityVeryBig5 = new FuzzyRuleAntecedent();
  ifhumidityVeryBig5->joinSingle(verybig5);
  thenHumValveLow5->addOutput(lowb5);
  FuzzyRule* fuzzyRule5 = new FuzzyRule(5, ifhumidityVeryBig5, thenHumValveLow5);
  fuzzy->addFuzzyRule(fuzzyRule5); 

  ifhumidityVerySmall5->joinSingle(verysmall5);
  FuzzyRuleConsequent *thenHumFanLow8 = new FuzzyRuleConsequent();
  thenHumFanLow8->addOutput(lowb8);
  FuzzyRule *fuzzyRule6 = new FuzzyRule(6, ifhumidityVerySmall5, thenHumFanLow8);
  fuzzy->addFuzzyRule(fuzzyRule6);
  

  ifhumiditySmall5->joinSingle(small5);
  thenHumFanLow8->addOutput(lowb8);
  FuzzyRule *fuzzyRule7 = new FuzzyRule(7, ifhumiditySmall5, thenHumFanLow8);
  fuzzy->addFuzzyRule(fuzzyRule7);

  
 
  ifhumidityMid5->joinSingle(mid5);
  FuzzyRuleConsequent *thenHumFanMid8 = new FuzzyRuleConsequent();
  thenHumFanMid8->addOutput(midb8);
  FuzzyRule *fuzzyRule8 = new FuzzyRule(8, ifhumidityMid5, thenHumFanMid8);
  fuzzy->addFuzzyRule(fuzzyRule8);


  ifhumidityBig5->joinSingle(big5);
  FuzzyRuleConsequent *thenHumFanHigh8 = new FuzzyRuleConsequent();
  thenHumFanHigh8->addOutput(highb8);
  FuzzyRule* fuzzyRule9 = new FuzzyRule(9, ifhumidityBig5, thenHumFanHigh8);
  fuzzy->addFuzzyRule(fuzzyRule9);
  
  
 
  ifhumidityVeryBig5->joinSingle(verybig5);
  FuzzyRuleConsequent *thenHumFanVeryHigh8 = new FuzzyRuleConsequent();
  thenHumFanVeryHigh8->addOutput(veryhighb8);
  FuzzyRule* fuzzyRule10 = new FuzzyRule(10, ifhumidityVeryBig5, thenHumFanHigh8);
  fuzzy->addFuzzyRule(fuzzyRule10); 

}
  float humidity() {
    float humidity = dht.readHumidity();
    return humidity;
}



void loop() {

 
  float hum = humidity();
  delay(3000);
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print(H + hum );
     Serial.print( hum );
   if (hum < 0 || hum > 100 ) return;

  
  fuzzy->setInput(1, hum);

  fuzzy->fuzzify();

  
  int output1 = fuzzy->defuzzify(1); 
  int output2 = fuzzy->defuzzify(2);
  
  analogWrite(Valve3, output1);
  analogWrite(EFan, output2);

}
