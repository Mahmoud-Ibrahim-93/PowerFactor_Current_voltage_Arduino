// Henry's Bench TA-1200 AC Current Sensor Tutorial


int sensorTA12 = A1; // Analog input pin that sensor is attached to

float nVPP;   // Voltage measured across resistor
float nCurrThruResistorPP; // Peak Current Measured Through Resistor
float nCurrThruResistorRMS; // RMS current through Resistor
float nCurrentThruWire;     // Actual RMS current in Wire

void setup() 
 {
   Serial.begin(115200); 
   pinMode(sensorTA12, INPUT);
 }
 
 
 void loop() 
 { 
   nVPP = getVPP();
   Serial.print("Current Through Wire : ");
   Serial.print(nVPP,3);
   Serial.println(" mA RMS");
 }


 /************************************ 
In order to calculate RMS current, we need to know
the peak to peak voltage measured at the output across the
200 Ohm Resistor

The following function takes one second worth of samples
and returns the peak value that is measured
 *************************************/
 
 
float getVPP()
{
  float result;
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
   uint32_t start_time = millis();
   while((millis()-start_time) < 20) //sample for 1 Sec
   {
       readValue = analogRead(sensorTA12);
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           /*record the maximum sensor value*/
           maxValue = readValue;
       }
   }
   
   // Convert the digital data to a voltage
   result = (maxValue * 5.0)/1023.0;
   if(result<.05){ // Compensate for Sensor offset
    result=0;
    }
    result=result*3535;
   return result;
 }
