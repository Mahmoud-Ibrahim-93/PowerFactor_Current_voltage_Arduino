/*
  Check youtube channel for Sources:
  youtube.com/c/MahmoudIbrahimchannel
  Library originally added 12 Feb 2019
  by Mahmoud Ibrahim & Ahmed Saleh
*/

// include the library code:
#include <LiquidCrystal.h>
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
//const int rs = 12, en = 11, d2 = 4, d3 = 5, d4 = 6, d5 = 7;
const int rs = 9, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
//LiquidCrystal lcd(rs, en, d2, d3, d4, d5);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte voltageAnalogPin = A0;
const byte sensorTA12 = A1;
const byte interruptPinCurrent = 2;
const byte interruptPinVolt = 3;
volatile float pfTime = 0;
volatile float VoltTime = 0;
volatile float powerFactor = 0;
volatile float Energy = 0;
volatile float lastPowerTime = 0;
const float frequency = 50.0;
volatile bool currentFlag = false;
volatile bool measureCurrentFlag = false;

float Vrms = 0;
float Irms = 0;

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.clear();
  VoltTime = millis();
  lastPowerTime=millis();
  pinMode(interruptPinCurrent, INPUT_PULLUP);
  pinMode(interruptPinVolt, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPinCurrent), detectRisingEdgeCurrent, RISING );
  attachInterrupt(digitalPinToInterrupt(interruptPinVolt), detectRisingEdgeVolt, RISING );
}

void loop() {
  getACVoltageCurrent();
  //printSerial();
  // 0 to plot Vrms value - 1 to plot Irms - 2 to plot powerFactor - 3 to plot totalPower
  plotValues(3);
  printlcd();

}

void printlcd() {
  float totalPower = Vrms * Irms * 1E-3 * abs(powerFactor);
  float powerTime = millis()-lastPowerTime;
  Energy= Energy+(totalPower*powerTime*1E-3*0.000277778);
  lastPowerTime=millis();
  lcd.setCursor(0, 0);
  lcd.print("V=");
  lcd.print("    ");
  lcd.setCursor(2, 0);
  lcd.print((int)Vrms,1);
  lcd.setCursor(0,1);
  lcd.print("I=");
  lcd.print("        ");
  lcd.setCursor(2, 1);
  lcd.print(Irms/1000.0);
  lcd.setCursor(7,0);
  lcd.print("PF=");
  lcd.print("      ");
  lcd.setCursor(10,0);
  lcd.print(powerFactor,2);
  lcd.setCursor(7,1);
  lcd.print("Pw=");
  lcd.print("      ");
  lcd.setCursor(10,1);
  lcd.print(totalPower);
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Energy Consumed:");
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(0,1);
  lcd.print(Energy,2);
  lcd.setCursor(14,1);
  lcd.print("wh");
  delay(1500);
}

void printSerial() {
  Serial.print("V RMS Voltage Value: ");
  Serial.print(Vrms);
  Serial.print(" I RMS Voltage Value: ");
  Serial.print(Irms);
  Serial.print(" Power Factor Value: ");
  Serial.println(powerFactor);
  Serial.print("TotalPower Consumed= ");
  float totalPower = Vrms * Irms * 1E-3 * abs(powerFactor);
  Serial.println(totalPower);
  delay(500);
}
void plotValues(int type) {
  switch (type) {
    case 0:
      Serial.println(Vrms);
      delay(10);
      break;
    case 1:
      Serial.println(Irms);
      delay(10);
      break;
    case 2:
      Serial.println(powerFactor);
      delay(10);
      break;
    default:
      float totalPower = Vrms * Irms * 1E-3 * abs(powerFactor);
      Serial.println(totalPower);
      delay(10);

      break;
      //delay(100);
  }
}
void detectRisingEdgeCurrent() {
  currentFlag = true;
  measureCurrentFlag=true;
  pfTime = millis() - VoltTime;
}
void detectRisingEdgeVolt() {
  VoltTime = millis();
  // first check for ac loads
  if (currentFlag) {
    currentFlag = false;
    powerFactor = cos(2 * (22 / 7.0) * pfTime * 1E-3 * frequency);
  } else {
    powerFactor = 0;
    measureCurrentFlag=false;
  }
  //Serial.println(powerFactor);
}

void getACVoltageCurrent() {
  int Vpeak = 0;
  int currentReadValue = 0;           //value read from the sensor
  int VmaxValue = 0;          // store max value here
  int Vreading = 0;
  uint32_t start_time = millis();

  while ((millis() - start_time) < 1000) //sample for 1 Sec
  {
    int Vreading = analogRead(voltageAnalogPin);
    delayMicroseconds(500);

    if (Vpeak < Vreading) {
      Vpeak = Vreading;
    }

    if(measureCurrentFlag){
      currentReadValue = analogRead(sensorTA12);
    }else{
      currentReadValue=0;
      }
    delayMicroseconds(500);

    // see if you have a new maxValue
    if (currentReadValue > VmaxValue)
    {
      /*record the maximum sensor value*/
      VmaxValue = currentReadValue;
    }
  }

  // Convert the digital data to a voltage
  Irms = (VmaxValue * 5.0) / 1023.0;


  Irms = Irms * 3535.0;
  Vrms = 0.27 * Vpeak;
}
