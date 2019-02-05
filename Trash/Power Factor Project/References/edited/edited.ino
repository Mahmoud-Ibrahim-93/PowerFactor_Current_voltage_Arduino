
int sensorTA12 = A0; // Analog input pin that sensor is attached to

float nVPP;   // Voltage measured across resistor
float nCurrThruResistorPP; // Peak Current Measured Through Resistor
float nCurrThruResistorRMS; // RMS current through Resistor
float nCurrentThruWire;     // Actual RMS current in Wire
 int mySensVals[100] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
void setup() 
 {
   Serial.begin(9600); 
   pinMode(sensorTA12, INPUT);
 }
 
 
 void loop() 
 {
   
   
   nVPP = getVPP();
   for (int i=0;i<100;i++){
    Serial.print(mySensVals[i]);
    Serial.print(" ");
    }
     Serial.println();
/*
   
   nCurrThruResistorPP = (nVPP/200.0) * 1000.0;
   

   
   nCurrThruResistorRMS = nCurrThruResistorPP * 0.707;
   
   
   nCurrentThruWire = nCurrThruResistorRMS * 1000;

   
   Serial.print("Volts Peak : ");
   Serial.println(nVPP,3);
 
   
   Serial.print("Current Through Resistor (Peak) : ");
   Serial.print(nCurrThruResistorPP,3);
   Serial.println(" mA Peak to Peak");
   
   Serial.print("Current Through Resistor (RMS) : ");
   Serial.print(nCurrThruResistorRMS,3);
   Serial.println(" mA RMS");
   
   Serial.print("Current Through Wire : ");
   Serial.print(nCurrentThruWire,3);
   Serial.println(" mA RMS");
   
   Serial.println();
    
   */
   

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
  int i=0;
  float result;
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
   uint32_t start_time = millis();
   //while((millis()-start_time) < 11) //sample for 1 Sec
   while(i<100) //sample for 1 Sec
   {
       readValue = analogRead(sensorTA12);
       delayMicroseconds(200);
       mySensVals[i]=readValue;
       i++;
       // Serial.println(readValue);
        /*
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           /*record the maximum sensor value*/
          // maxValue = readValue;
     //  }
   }
   
   // Convert the digital data to a voltage
   //result = (maxValue * 5.0)/1024.0;
    return mySensVals[i-1];
   //return result;
 }
