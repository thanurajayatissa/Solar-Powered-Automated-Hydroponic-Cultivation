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

#define PUMP 7
#define sensor A1

Fuzzy *fuzzy = new Fuzzy();

int gas, co2lvl;

String carbon = "CO2Level:";


void setup() {
   pinMode(sensor, INPUT);
   pinMode(PUMP, OUTPUT);
  Serial.begin(9600);

  lcd.clear();
  lcd.init();                      
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("HYDROPONIC");

  delay(100);
  lcd.clear();
  lcd.print("Warming coil");

  for(int i =0;i<=100;i++)
  {
    lcd.setCursor(12,0);
    if (i<100) lcd.print(" ");
    if (i<10) lcd.print(" ");
    lcd.print(i);
    lcd.print("%");
    delay(700);
  }

  FuzzySet *verysmall1 = new FuzzySet(0, 150, 150, 300);
  FuzzySet *small1 = new FuzzySet(100, 300, 300, 600);
  FuzzySet *mid1 = new FuzzySet(300, 600, 600, 900);
  FuzzySet *big1 = new FuzzySet(600, 900, 900, 1200);
  FuzzySet *verybig1 = new FuzzySet(900, 1200, 1200, 1500);

  

  FuzzySet *lowb1 = new FuzzySet(0, 50, 50, 100);
  FuzzySet *midb1 = new FuzzySet(50, 100, 100, 150);
  FuzzySet *highb1 = new FuzzySet(100, 150, 150, 200);
  FuzzySet *veryhighb1 = new FuzzySet(150, 200, 200, 255);
  

  FuzzyInput *AirState = new FuzzyInput(1);
  AirState->addFuzzySet(verysmall1);
  AirState->addFuzzySet(small1);
  AirState->addFuzzySet(mid1);
  AirState->addFuzzySet(big1);
  AirState->addFuzzySet(verybig1);
  fuzzy->addFuzzyInput(AirState);

  FuzzyOutput *pumpSpeed = new FuzzyOutput(1);
  pumpSpeed->addFuzzySet(lowb1);
  pumpSpeed->addFuzzySet(midb1);
  pumpSpeed->addFuzzySet(highb1);
  pumpSpeed->addFuzzySet(veryhighb1);
  fuzzy->addFuzzyOutput(pumpSpeed);

  FuzzyRuleAntecedent *ifAirStateVerySmall1 = new FuzzyRuleAntecedent();
  ifAirStateVerySmall1->joinSingle(verysmall1);
  FuzzyRuleConsequent *thenpumpSpeedLow1 = new FuzzyRuleConsequent();
  thenpumpSpeedLow1->addOutput(lowb1);
  FuzzyRule *fuzzyRule1 = new FuzzyRule(1, ifAirStateVerySmall1, thenpumpSpeedLow1);
  fuzzy->addFuzzyRule(fuzzyRule1);
  
  FuzzyRuleAntecedent *ifAirStateSmall1 = new FuzzyRuleAntecedent();
  ifAirStateSmall1->joinSingle(small1);
  thenpumpSpeedLow1->addOutput(lowb1);
  FuzzyRule *fuzzyRule2 = new FuzzyRule(2, ifAirStateSmall1, thenpumpSpeedLow1);
  fuzzy->addFuzzyRule(fuzzyRule2);

  
 
  FuzzyRuleAntecedent *ifAirStateMid1 = new FuzzyRuleAntecedent();
  ifAirStateMid1->joinSingle(mid1);
  FuzzyRuleConsequent *thenpumpSpeedMid1 = new FuzzyRuleConsequent();
  thenpumpSpeedMid1->addOutput(midb1);
  FuzzyRule *fuzzyRule3 = new FuzzyRule(3, ifAirStateMid1, thenpumpSpeedMid1);
  fuzzy->addFuzzyRule(fuzzyRule3);

  
  FuzzyRuleAntecedent *ifAirStateBig1 = new FuzzyRuleAntecedent();
  ifAirStateBig1->joinSingle(big1);
  FuzzyRuleConsequent *thenpumpSpeedHigh1 = new FuzzyRuleConsequent();
  thenpumpSpeedHigh1->addOutput(highb1);
  FuzzyRule* fuzzyRule4 = new FuzzyRule(4, ifAirStateBig1, thenpumpSpeedHigh1);
  fuzzy->addFuzzyRule(fuzzyRule4);
  
  
  FuzzyRuleAntecedent *ifAirStateVeryBig1 = new FuzzyRuleAntecedent();
  ifAirStateVeryBig1->joinSingle(verybig1);
  FuzzyRuleConsequent *thenpumpSpeedVeryHigh1 = new FuzzyRuleConsequent();
  thenpumpSpeedVeryHigh1->addOutput(veryhighb1);
  FuzzyRule* fuzzyRule5 = new FuzzyRule(5, ifAirStateVeryBig1, thenpumpSpeedHigh1);
  fuzzy->addFuzzyRule(fuzzyRule5); 

}

float Co2(){
  gas = analogRead(sensor);
  co2lvl = gas-120;
  co2lvl = map(co2lvl,0,1024,400,5000);
  return co2lvl;
}

void loop() {
 float Air = Co2();
  delay(3000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print( carbon + Air );
Serial.println( Air );

if((co2lvl >= 350)&&(co2lvl <= 1000))
  {
    lcd.setCursor(0,1);
    lcd.print("Good level");
  }
  else if((co2lvl >= 1000)&&(co2lvl <= 2000))
  {
    lcd.setCursor(0,1);
    lcd.print("Bad");
  }
if (Air < 0 || Air > 1600 ) return;

  
  fuzzy->setInput(1, Air);

  fuzzy->fuzzify();

  
  int output = fuzzy->defuzzify(1); 
  
  analogWrite(PUMP, output);

}
