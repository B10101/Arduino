#include "HX711.h"   
const int DOUT = 2;
const int CLK = 3;
HX711 scale(); 
float calibration_factor = -560; //-560 works for my 10kg loadcell. 
void setup() { 
 Serial.begin(9600); 
 Serial.println("HX711 Rocket Motor Dynamometer, V.5"); 
 scale.begin(DOUT,CLK);
 scale.set_scale(); 
 scale.tare(); //Reset the scale to 0 
 long zero_factor = scale.read_average(); //Get a baseline reading 
 Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects. 
 Serial.println(zero_factor); 
 Serial.println(" "); 
} 
void loop() { 
 scale.set_scale(calibration_factor); 
 buttonState = digitalRead(buttonPin);  
 Serial.println("IGNITION SEQUENCE ACTIVATED!"); 
 for (int i=0; i <= 50; i++){ 
 delay (100);  delay (100);  
 Serial.print("Start time, ms: "); 
 Serial.print (millis()); 
 Serial.println(" "); 
 Serial.println(); 
 for (int i=0; i <= 800; i++){ //800 samples at 80sa/sec = 10 seconds theoretical 
 scale.set_scale(calibration_factor); //Adjust to the calibration factor 
 Serial.print(scale.get_units(), 1); 
 Serial.println(); 
 } 
 Serial.println(); 
 Serial.print("Stop Time, ms: "); 
 Serial.print(millis());  
 Serial.println(); 
 } 
}
