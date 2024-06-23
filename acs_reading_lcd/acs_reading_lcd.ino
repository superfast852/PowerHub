/*
 * 
 * Arduino Sketch for Allegro ACS712 Current Sensor 
 * this sensor is offered at 5A, 20A and 30A range for AC and DC
 
 * It operates with 5V
 * Please watch video instruction and explanation for this code
  Watch video instruction for ACS770 50A to 200A Sensors https://youtu.be/sB6EULTix2k
  Watch video instruction for ACS712 5A to 30A Sensors :https://youtu.be/3C33DpcSwIw
 
 
ACS712 Video Timing
00:51 Introduction
01:31 Hardware Explained
02:20 Datasheet Viewed
05:55 Wiring Explained
      Get wiring diagram from my Arduino Course
07:36 Code settings explained (library download shown)
      Full explanation of code at my Arduino Course
12:28 5A module demonstration
14:35 30A module demonstration
17:07 AC current measurement
19:37 Conclusion remarks
 * 
 * Written by Ahmad Shamshiri on Saturday Mar 14, 2020 at 16:15 in Ajax, Ontario, Canada
 * for Robojax.com
 * View the video :
 * This code has been downloaded from Robojax.com
 * 
model select
      //ACS712
      0-"ACS712ELCTR-05B",
      1-"ACS712ELCTR-20A",
      2-"ACS712ELCTR-30A",
      
      //ACS758
      3,// for ACS758LCB-050B
      4,// for ACS758LCB-050U
      5,// for ACS758LCB-100B
      6,// for ACS758LCB-100U
      7,// for ACS758KCB-150B
      8,// for ACS758KCB-150U
      9,// for ACS758ECB-200B
      10,// for ACS758ECB-200U 
      
      ///ACS770
      11,// for ACS770x-050B ///      
      12,// for ACS770x-050U  
      13,// for ACS770x-100B
      14,// for ACS770x-100U
      15,// for ACS770x-150B
      16,// for ACS770x-150U
      17,// for ACS770x-200B  
      18,// for ACS770x-200U  
      

      19 for "ACS732KLATR-20AB",
      20 for "ACS732KLATR-40AB",
      21 for "ACS732KLATR-65AB", 
      22 for "ACS732KLATR-65AU",   
      23 for "ACS732KLATR-75AB", 

      24 for "ACS733KLATR-20AB",
      25 for "ACS733KLATR-40AB",
      26 for "ACS733KLATR-40AU", 
      27 for "ACS733KLATR-65AU",

 * this code has been downloaded from http://robojax.com/learn/arduino/
 
 * Get this code and other Arduino codes from Robojax.com
Learn Arduino step by step in structured course with all material, wiring diagram and library
all in once place. Purchase My course on Udemy.com http://robojax.com/L/?id=62

****************************
Get early access to my videos via Patreon and have  your name mentioned at end of very 
videos I publish on YouTube here: http://robojax.com/L/?id=63 (watch until end of this video to list of my Patrons)
****************************

or make donation using PayPal http://robojax.com/L/?id=64

 *  * This code is "AS IS" without warranty or liability. Free to be used as long as you keep this note intact.* 
 * This code has been download from Robojax.com
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

 */
const int VIN = 3; // define the Arduino pin A0 as voltage input (V in)
const float VCC   = 5;// supply voltage
const int MODEL = 1;   // enter the model (see above list)
#include <Robojax_AllegroACS_Current_Sensor.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

Robojax_AllegroACS_Current_Sensor robojax(MODEL,VIN);

void setup() {
    analogReadResolution(10);
    //Robojax.com Allegro ACS Current Sensor 
    Serial.begin(9600);// initialize serial monitor
    Serial.println("Initiated!");
    lcd.init();
    lcd.backlight();
	
}

void loop() {
   //Robojax.com Allegro ACS Current Sensor
   int storage = robojax.getCurrent();
   delay(100);
   int calculate = robojax.getCurrent() - storage;
    if (robojax.getCurrent() - calculate <= 0.2){
      lcd.setCursor(0,0);
      lcd.print("Current: ");
      lcd.print(robojax.getCurrent()); // print the current with 3 decimal places
      lcd.setCursor(0,1);
      lcd.print("A Avg: ");
      lcd.print(robojax.getCurrentAverage(300));//   
      lcd.print("A");
    }
    delay(500);
}
 
