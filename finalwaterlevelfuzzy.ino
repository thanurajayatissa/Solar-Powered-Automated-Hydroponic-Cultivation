#include <Fuzzy.h>
#include <FuzzyComposition.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzyOutput.h>
#include <FuzzyRule.h>
#include <FuzzyRuleAntecedent.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzySet.h>

#include <Arduino.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);


#define TRIGGER 4
#define ECHO 5
#define Valve2 6


String W = "waterLevel:";
int Height = 25;

Fuzzy *fuzzy = new Fuzzy();

void setup() {

  lcd.clear();
  lcd.init();                      
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("HYDROPONIC");
  
  Serial.begin(9600);
 
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(Valve2, OUTPUT);

  FuzzySet *verysmall3 = new FuzzySet(0, 2, 2, 4);
  FuzzySet *small3 = new FuzzySet(2, 4, 4, 6);
  FuzzySet *mid3 = new FuzzySet(4, 8, 8, 12);
  FuzzySet *big3 = new FuzzySet(8, 13, 13, 18);
  FuzzySet *verybig3 = new FuzzySet(13, 19, 19, 25);

  FuzzySet *lowb3 = new FuzzySet(0, 50, 50, 100);
  FuzzySet *midb3 = new FuzzySet(50, 100, 100, 150);
  FuzzySet *highb3 = new FuzzySet(100, 150, 150, 200);
  FuzzySet *veryhighb3 = new FuzzySet(150, 200, 200, 255);

  FuzzyInput *waterlevel = new FuzzyInput(1);
  waterlevel->addFuzzySet(verysmall3);
  waterlevel->addFuzzySet(small3);
  waterlevel->addFuzzySet(mid3);
  waterlevel->addFuzzySet(big3);
  waterlevel->addFuzzySet(verybig3);
  fuzzy->addFuzzyInput(waterlevel);

  FuzzyOutput *WaterValve = new FuzzyOutput(1);
  WaterValve->addFuzzySet(lowb3);
  WaterValve->addFuzzySet(midb3);
  WaterValve->addFuzzySet(highb3);
  WaterValve->addFuzzySet(veryhighb3);
  fuzzy->addFuzzyOutput(WaterValve);

 
  FuzzyRuleAntecedent *ifwaterlevelVerySmall3 = new FuzzyRuleAntecedent();
  ifwaterlevelVerySmall3->joinSingle(verysmall3);
  FuzzyRuleConsequent *thenWaterValveLow3 = new FuzzyRuleConsequent();
  thenWaterValveLow3->addOutput(lowb3);
  FuzzyRule *fuzzyRule1 = new FuzzyRule(1, ifwaterlevelVerySmall3, thenWaterValveLow3);
  fuzzy->addFuzzyRule(fuzzyRule1);
  
  FuzzyRuleAntecedent *ifwaterlevelSmall3 = new FuzzyRuleAntecedent();
  ifwaterlevelSmall3->joinSingle(small3);
  thenWaterValveLow3->addOutput(lowb3);
  FuzzyRule *fuzzyRule2 = new FuzzyRule(2, ifwaterlevelSmall3, thenWaterValveLow3);
  fuzzy->addFuzzyRule(fuzzyRule2);

  
 
  FuzzyRuleAntecedent *ifwaterlevelMid3 = new FuzzyRuleAntecedent();
  ifwaterlevelMid3->joinSingle(mid3);
  FuzzyRuleConsequent *thenWaterValveMid3 = new FuzzyRuleConsequent();
  thenWaterValveMid3->addOutput(midb3);
  FuzzyRule *fuzzyRule3 = new FuzzyRule(3, ifwaterlevelMid3, thenWaterValveMid3);
  fuzzy->addFuzzyRule(fuzzyRule3);

  
  FuzzyRuleAntecedent *ifwaterlevelBig3 = new FuzzyRuleAntecedent();
  ifwaterlevelBig3->joinSingle(big3);
  FuzzyRuleConsequent *thenWaterValveHigh3 = new FuzzyRuleConsequent();
  thenWaterValveHigh3->addOutput(highb3);
  FuzzyRule* fuzzyRule4 = new FuzzyRule(4, ifwaterlevelBig3, thenWaterValveHigh3);
  fuzzy->addFuzzyRule(fuzzyRule4);
  
  
  FuzzyRuleAntecedent *ifwaterlevelVeryBig3 = new FuzzyRuleAntecedent();
  ifwaterlevelVeryBig3->joinSingle(verybig3);
  FuzzyRuleConsequent *thenWaterValveVeryHigh3 = new FuzzyRuleConsequent();
  thenWaterValveVeryHigh3->addOutput(veryhighb3);
  FuzzyRule* fuzzyRule5 = new FuzzyRule(5, ifwaterlevelVeryBig3, thenWaterValveHigh3);
  fuzzy->addFuzzyRule(fuzzyRule5); 

}
  int waterlevel() {
  digitalWrite(TRIGGER,LOW);

  delay(1);

  digitalWrite(TRIGGER,HIGH);

  delay(1);

  digitalWrite(TRIGGER,LOW);



  long t = pulseIn(ECHO,HIGH);



  

  long cm = t / 29 / 2;
  return cm;
}



void loop() {

 
int WL = waterlevel();

  delay(3000);
  
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print( W + (Height-WL) );
     
if (WL < 0 || WL > 25 ) return;


  fuzzy->setInput(1, WL);
  fuzzy->fuzzify();

  
  int output = fuzzy->defuzzify(1); 
  
  analogWrite(Valve2, output);

}
