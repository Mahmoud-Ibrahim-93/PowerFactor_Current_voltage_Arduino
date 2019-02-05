

void setup()
{
Serial.begin(115200);
Serial.println("Voltage: ");
Serial.print("V");
}

void loop()
{
  int peak=0;
float temp;
for(int i=0;i<400;i++){
int Reading=analogRead(A1);
  if(peak<Reading){
    peak=Reading;
  }
  delayMicroseconds(500);
  }
  peak=0.27*peak;
Serial.println(peak);
}
