/* How to use the HC-SR04 Ultrasonic Sensor with Arduino 
   Dev: Michalis Vasilakis // Date: 23/7/2015 // www.ardumotive.com */

//Libraries
#include "Ultrasonic.h"


#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F, 16, 2);

#include "HX711.h"
#include <Wire.h> 

#define DOUT  A3
#define CLK   A2

#define DEC_POINT  2
#define STABLE  1

#define STOPPIN 2

float offset=-6.39;
float calibration_factor = 8076;

HX711 scale(DOUT, CLK);

unsigned char stop_read = 0;

float get_units_kg();
void  ReadWeight();

void setup()
{
  // initialize the LCD
  lcd.begin();
  lcd.print("Smart Mesure");
  lcd.setCursor(0, 1);
  lcd.print("Pongsri Vitaya");
  lcd.clear();
}

//Define pins ultrasonic(trig,echo)
Ultrasonic ultrasonic(A0,A1);

//Variables
int distance;
int read_stop = 0;

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


void loop()
{
    distance = ultrasonic.Ranging(CM); //Use 'CM' for centimeters or 'INC' for inches
  //Print distance...
  int stop_read = digitalRead(STOPPIN); 

  if (stop_read != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
     if (stop_read != buttonState) {
      buttonState = stop_read;

      // only toggle the LED if the new button state is HIGH
      if (stop_read == HIGH) {
        read_stop = !read_stop;
      }
    }
  }
  
  //if (stop_read == HIGH) {
    // stop display
  //  read_stop = !read_stop;   
 // }

  if ( !read_stop ) {
  
    lcd.setCursor(0, 0);
    lcd.print("Height: ");
    String heightbody  = String(200-distance);
    lcd.print(heightbody);
    lcd.print(" cm ");
    lcd.setCursor(0, 1);
    ReadWeight();
  }
  //every 1sec. 
  //delay(500);
   lastButtonState = stop_read;
}

float get_units_kg()
{
  return(scale.get_units()*0.453592);
}
void ReadWeight()
{
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  lcd.setCursor(0, 1);
  lcd.print("Weight: ");
  String data = String(get_units_kg()+offset, DEC_POINT);
  lcd.print(data);
  lcd.print(" kg ");
}
