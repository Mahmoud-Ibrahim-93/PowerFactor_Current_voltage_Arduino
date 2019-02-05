const byte voltageAnalogPin = A0;
const byte interruptPinCurrent = 2;
const byte interruptPinVolt = 3;
volatile float pfTime = 0;
volatile float VoltTime = 0;
volatile float powerFactor = 0;
const float frequency = 50.0;
volatile bool currentFlag = false;
int Vrms = 0;

void setup() {
  Serial.begin(115200);
  VoltTime = millis();
  pinMode(interruptPinCurrent, INPUT_PULLUP);
  pinMode(interruptPinVolt, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPinCurrent), detectRisingEdgeCurrent, RISING );
  attachInterrupt(digitalPinToInterrupt(interruptPinVolt), detectRisingEdgeVolt, RISING );
}

void loop() {
  getACVoltage();
  Serial.print("RMS Voltage Value: ");
  Serial.print(Vrms);
  Serial.print(" Power Factor Value: ");
  Serial.println(powerFactor);
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

void getACVoltage() {
  int peak = 0;
  Vrms = 0;
  for (int i = 0; i < 400; i++) {
    int Reading = analogRead(voltageAnalogPin);
    if (peak < Reading) {
      peak = Reading;
    }
    delayMicroseconds(500);
  }
  Vrms = 0.27 * peak;
}
