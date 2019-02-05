const byte ledPin = 13;
const byte interruptPinCurrent = 2;
const byte interruptPinVolt = 3;
volatile float pfTime = 0;
volatile float VoltTime = 0;
volatile float powerFactor = 0;
const float frequency = 50.0;
volatile bool currentFlag = false;
void setup() {
  Serial.begin(115200);
  VoltTime = millis();
  pinMode(interruptPinCurrent, INPUT_PULLUP);
  pinMode(interruptPinVolt, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPinCurrent), detectRisingEdgeCurrent, RISING );
  attachInterrupt(digitalPinToInterrupt(interruptPinVolt), detectRisingEdgeVolt, RISING );
}

void loop() {
}

void detectRisingEdgeCurrent() {
  currentFlag = true;
  pfTime = millis() - VoltTime;
}
void detectRisingEdgeVolt() {
  VoltTime = millis();
  if (currentFlag) {
    currentFlag=false;
    powerFactor = cos(2 * (22 / 7.0) * pfTime * 1E-3 * frequency);
  } else {
    powerFactor = 0;
  }
  Serial.println(powerFactor);
}
