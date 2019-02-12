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
  plotValues(1);

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
      delay(100);
  }
}
void detectRisingEdgeCurrent() {
  currentFlag = true;
  pfTime = millis() - VoltTime;
}
void detectRisingEdgeVolt() {
  VoltTime = millis();
  if (currentFlag) {
    currentFlag = false;
    powerFactor = cos(2 * (22 / 7.0) * pfTime * 1E-3 * frequency);
  } else {
    powerFactor = 0;
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
  if (Irms < .05) { // Compensate for Sensor offset
    Irms = 0;
  }

  Irms = Irms * 3535.0;
  Vrms = 0.27 * Vpeak;
}
