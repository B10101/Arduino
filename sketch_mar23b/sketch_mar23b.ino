
#include <Wire.h>


#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

int ButtonValue = 0;
int Button = 3;
int LEDRED = 4;
int LEDORANGE = 2;

const int HX711_dout = 9; //mcu > HX711 dout pin
const int HX711_sck = 8; //mcu > HX711 sck pin

HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;

enum states
{
    ST_IDLE = 0,
    ST_DO_TARE,
    ST_WARNING_DELAY,
    ST_COLLECTING,
    ST_DONE_COLLECTING
            
};



void setup() 
{
    pinMode(Button, INPUT);
    pinMode (LEDRED, OUTPUT);
    pinMode (LEDORANGE, OUTPUT);
    pinMode(10,OUTPUT);
    Serial.begin(9600); delay(10);
    Serial.println();
    Serial.println("Starting");
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Starting");
    
    LoadCell.begin();
    float calibrationValue; // calibration value (see example file "Calibration.ino")
    calibrationValue = 696.0; // uncomment this if you want to set the calibration value in the sketch
    #if defined(ESP8266)|| defined(ESP32)
    //EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
    #endif
    EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom
    
    unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
    boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
    LoadCell.start(stabilizingtime, _tare);
    if (LoadCell.getTareTimeoutFlag()) 
    {
        Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
        while (1);
    }
    else 
    {
        LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
        Serial.println("Stand By");
        lcd.clear();
        lcd.print("Stand By");
    }
}

void loop() 
{
    static uint32_t
        timeSample;
    static uint8_t
        sampleIndex,
        state = ST_IDLE;
    uint32_t
        timeNow = micros();

    switch( state )
    {
        case    ST_IDLE:
            //in idle state check if tare is requested
            if (Serial.available() > 0) 
            {
                char inByte = Serial.read();
                if (inByte == 't') 
                {
                    while( Serial.available() )
                        Serial.read();

                    Serial.println("Performing Tare...");
                    LoadCell.tareNoDelay();
                    
                    state = ST_DO_TARE;

                }//if
                
            }//if            
            else if( digitalRead( Button ) == HIGH )
            {
                //button was pressed
                timeSample = timeNow;                
                digitalWrite(LEDRED, HIGH);
                lcd.setCursor(0,0);
                lcd.print("Timer Activated");
                Serial.println("Timer Activated");
                lcd.setCursor(0,1);
                lcd.print("Stand Back!");
                Serial.println("Stand Back!");

                state = ST_WARNING_DELAY;
                
            }//if
            
        break;

        case    ST_DO_TARE:
            // check if last tare operation is complete:
            if (LoadCell.getTareStatus() == true) 
            {
                Serial.println("Tare complete");
                state = ST_IDLE;
                            
            }//if
        
        break;

        case    ST_WARNING_DELAY:
            //after a 10-second delay...
            if( (timeNow - timeSample) >= 10000000ul )    //10-seconds (10000000uS)
            {
                //do this stuff
                lcd.clear();
                digitalWrite(LEDRED, LOW);
                digitalWrite(LEDORANGE, HIGH);
                digitalWrite(10,HIGH);
                
                lcd.setCursor(0,0);
                lcd.print("Collecting Data");
                Serial.println("Collecting Data");

                Serial.print( "Time " ); 
                Serial.write( '\t' );
                Serial.println( "Grams " );

                //and move to collect data state
                sampleIndex = 0;
                timeSample = timeNow;
                state = ST_COLLECTING;
                
            }//if
        
        break;

        case    ST_COLLECTING:
            //every 10,000uS or 10mS, collect a data point
            if( (timeNow - timeSample) >= 10000ul ) //10mS for 100 samples/sec
            {
                ///...if datd ready
                if( LoadCell.update() )
                {
                    float g = LoadCell.getData();
                    timeSample = timeNow;
                    Serial.print( timeNow ); 
                    Serial.write( '\t' );
                    Serial.println( g, 3 );
                    sampleIndex++;
                    //after 100 samples...
                    if( sampleIndex == 100 )
                    {
                        //move to done collection state
                        state = ST_DONE_COLLECTING;
                        
                    }//if
                                        
                }//if
                
            }//else
            
        break;

        case    ST_DONE_COLLECTING:
            //8-second delay
            if( (timeNow - timeSample) >= 8000000ul )   //8-seconds
            {
                //then do this
                lcd.clear();
                digitalWrite(LEDORANGE, LOW);
                digitalWrite(10,LOW);
                lcd.setCursor(0,0);
                lcd.print("Complete");
                Serial.println("Complete");

                state = ST_IDLE;
                    
            }//if
            
        break;
        
    }//switch
    
}//loop
