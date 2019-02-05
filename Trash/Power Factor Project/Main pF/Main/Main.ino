// Code written by Jamie Bunnell
// Technical Services Engineer 11/09/2017
// smooth_jamie@outlook.com


// Define mathematical constants

int pin = A0;
float rads = 57.29577951; // 1 radian = approx 57 deg.
float degree = 360;
float frequency = 50;
float nano = 1 * pow (10,-6); // Multiplication factor to convert nano seconds into seconds

// Define floats to contain calculations

float pf;
float angle;
float pf_max = 0;
float angle_max = 0;
int ctr;

void setup()
{
 //pinMode(pin, INPUT);
 Serial.begin(115200);
}

void loop()
{
for (ctr = 0; ctr <=500; ctr++) // Perform 4 measurements then reset
  {
  // 1st line calculates the phase angle in degrees from differentiated time pulse
  // Function COS uses radians not Degree's hence conversion made by dividing angle / 57.2958
   angle = ((((pulseIn(pin, HIGH)) * nano)* degree)* frequency);
   //delay(1);
  // pf = cos(angle / rads); 
   
   if (angle > angle_max) // Test if the angle is maximum angle
     {
      angle_max = angle; // If maximum record in variable "angle_max"
      pf_max = cos(angle_max / rads); // Calc PF from "angle_max"
     }
   }
   if (angle_max > 90 || angle_max< 0) // If the calculation is higher than 360 do following...
   {
    angle_max = 0; // assign the 0 to "angle_max"
    pf_max = 1; // Assign the Unity PF to "pf_max"
   }
   Serial.print(angle_max, 2); // Print the result
   Serial.print(",");
   Serial.println(pf_max, 2);
   //delay(500);
   angle = 0; // Reset variables for next test
   angle_max = 0;
}
