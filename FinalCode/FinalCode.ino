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
const int rs = 12, en = 11, d2 = 7, d3 = 6, d4 = 5, d5 = 4;
LiquidCrystal lcd(rs, en, d2, d3, d4, d5);

const byte voltageAnalogPin = A0;
const byte sensorTA12 = A1;
const byte interruptPinCurrent = 2;
const byte interruptPinVolt = 3;
volatile float pfTime = 0;
volatile float VoltTime = 0;
volatile float powerFactor = 0;
const float frequency = 50.0;
volatile bool currentFlag = false;
float Vrms = 0;
float Irms = 0;

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.clear();
  VoltTime = millis();
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
  //printlcd();

}

void printlcd() {
  lcd.setCursor(0, 0);
  lcd.print("VRMS=");
  lcd.setCursor(5, 0);
  lcd.print((int)Vrms);
  lcd.setCursor(9,0);
  lcd.print("I=");
  lcd.print(Irms/1000.0);
  lcd.setCursor(0,1);
  lcd.print("PF=");
  lcd.setCursor(4,1);
  lcd.print(powerFactor,2);
  float totalPower = Vrms * Irms * 1E-3 * abs(powerFactor);
  lcd.setCursor(9,1);
  lcd.print("Pw=");
  lcd.print(totalPower);
  delay(500);
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
  pfTime = millis() - VoltTime;
}
void detectRisingEdgeVolt() {
  VoltTime = millis();
  // first check for ac loads
  if (currentFlag) {
    currentFlag = false;
    powerFactor = cos(2 * (22 / 7.0) * pfTime * 1E-3 * frequency);
  } else {
    powerFactor = 1;
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
    currentReadValue = analogRead(sensorTA12);
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
  if (Irms < .055 ) { // Compensate for Sensor offset
    Irms = 0;
  }

  Irms = Irms * 3535.0;
  Vrms = 0.27 * Vpeak;
}
