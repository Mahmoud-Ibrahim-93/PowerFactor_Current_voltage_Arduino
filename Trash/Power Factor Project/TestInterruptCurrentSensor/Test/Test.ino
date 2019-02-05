const byte ledPin = 13;
const byte interruptPin = 3;
volatile float pulseTime = 0;
volatile float oldTime = 0;

void setup() {
  Serial.begin(115200);
  oldTime = millis();
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin),detectRisingEdgeCurrent, RISING );
}

void loop() {
delay(5);
}

void detectRisingEdgeCurrent() {
  
  pulseTime = millis()-oldTime;
  oldTime=millis();
  Serial.println(pulseTime);

}
