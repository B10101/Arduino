#include "HX711.h"  //Install this library from library manager
#include <Wire.h>                          
#include <LiquidCrystal_I2C.h>            //Including I2C LCD library
LiquidCrystal_I2C lcd(0x27, 16, 2);       //Creating an object of LCD_I2C
#define  Clock 5
#define Data 6                           //Defining Clock and Data Pins
HX711 scale;                              //Making an object of HX711 class
//Assume a random conversion factor and then this is changed according to the calibration
float calibration_factor = -109000;       //Assuming a calibration_factor
float weight;                            
void setup() {                            //Setup Function
  scale.begin(Data, Clock);               //Using Hx711 function scale to begin the connection
  Serial.begin(9600);                    
  Serial.println("HX711 Calibration");  
  Serial.println("First Remove all the weight");
  Serial.println("PLace your weight after the readings start");
  Serial.println("Calibration is increased when you press a,b,c,d by the factor of 10,100,1000,10000");
  Serial.println("Calibration is increased when you press e,f,g,h by the factor of 10,100,1000,10000");
  Serial.println("Press t for reset");    //Printing necessary instructions
  scale.set_scale();                  
  scale.tare();                           //Reset the scale to 0  
 
  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: ");          //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
  Wire.begin(7,8);                     //Starting Connection of LCD
  lcd.begin(16,2);
  lcd.setCursor(1,1);                    //Setting the LCD cursor to display value at a particular spot
  lcd.print("Weighing Scale");
  delay(3000);    
  lcd.clear();                           //Clearing the LCD after the delaying
    
}
void loop() {
    scale.set_scale(calibration_factor); //We are adjusting the calibration factor
 
    Serial.print("Reading Values ");
    Serial.print(scale.get_units(), 3);
    Serial.print(" kg"); //Changing this unit to KG to follow SI units
    Serial.print(" calibration_factor: ");
    Serial.print(calibration_factor);
    Serial.println();
    if(Serial.available()) //Applying the condition to check if something is entered on the serial or not
    {
        char temp = Serial.read();      //Reading the value entered on the serial monitor
        if(temp == '+' || temp == 'a')
            calibration_factor += 10;       //Applying if statements to increase or decrease the calibration factor according to requirement
        else if(temp =='-' || temp == 'e')
            calibration_factor -= 10;
        else if(temp == 'b')
            calibration_factor += 100;
        else if(temp == 'f')
            calibration_factor -= 100;
        else if(temp == 'c')
            calibration_factor += 1000;
        else if(temp == 'g')
            calibration_factor -= 1000;
        else if(temp == 'd')
            calibration_factor += 10000;      //Calibration factor will increase or decrease according to the key pressed
        else if(temp == 'h')
            calibration_factor -= 10000;    //Once we get right value of the weight placed we simplyset that thing as calibration factor
        else if(temp == 't')
            scale.tare();
    }
}
