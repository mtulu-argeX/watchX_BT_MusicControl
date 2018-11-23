#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "BluefruitConfig.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_DC     A3
#define OLED_CS     A5
#define OLED_RESET  A4
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS); 

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#define B1 10
#define B2 8
#define B3 11

#define LEDL 13
#define LEDR 6

#define BUZZER 9

#define ADAFRUITBLE_REQ A2
#define ADAFRUITBLE_RDY 0     // This should be an interrupt pin, on Uno thats #2 or #3
#define ADAFRUITBLE_RST A1

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

String ble_in; 

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

bool ptoggle = 0;

void draw(int control){

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(25,0);
    display.clearDisplay();
    display.println("MUSIC CONTROL");

    switch(control){

        case 0:
             display.setTextSize(2);   
             display.setCursor(35,30);
             display.println("PAUSE");

             tone(BUZZER, 1000, 300);

             break;
             
        case 1:
             display.setTextSize(2);   
             display.setCursor(39,30);
             display.println("PLAY");
             
             tone(BUZZER, 1000, 100);
             
             break;

        case 2:
             display.setTextSize(2);
             display.setCursor(17,20);
             display.println("PREVIOUS");
             display.setCursor(39,40);
             display.println("SONG");

             tone(BUZZER, 800, 100);

             break;

        case 3:
             display.setTextSize(2);              
             display.setCursor(39,20);
             display.println("NEXT");   
             display.setCursor(39,40);
             display.println("SONG");

             tone(BUZZER, 1100, 100);
             
             break;

         default:
             display.setTextSize(1);
             display.setTextColor(WHITE);
             display.setCursor(25,0);
             display.clearDisplay();
             display.println("MUSIC CONTROL");
      }
               
      display.display();
  }

void setup(void)
{

 display.begin(SSD1306_SWITCHCAPVCC);
 // Clear the buffer.
 display.clearDisplay();
  
 if ( !ble.begin(VERBOSE_MODE) )
  {
   error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  /* Disable command echo from Bluefruit */
  ble.echo(false);
  ble.verbose(false);  // debug info is a little annoying after this point!

  ble.factoryReset();
  ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=watchX" ));
  ble.sendCommandCheckOK(F( "AT+BleHIDEn=On"  ));
  ble.sendCommandCheckOK(F( "AT+BLEPOWERLEVEL=4" ));
  
  ble.reset();

  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(B3, INPUT_PULLUP);
  pinMode(6, OUTPUT);

  draw(4);
  
}

void loop(void)
{

    if(digitalRead(B1) == LOW){

      ble.sendCommandCheckOK(F("AT+BLEHIDCONTROLKEY=MEDIAPREVIOUS"));
      draw(2);
      digitalWrite(LEDL, HIGH);
      delay(50);
      digitalWrite(LEDL, LOW);
      
    }

    delay(50);

    if(digitalRead(B2) == LOW){

      ble.sendCommandCheckOK(F("AT+BLEHIDCONTROLKEY=PLAYPAUSE"));
      
      draw(ptoggle);
      ptoggle = !ptoggle;
      
      digitalWrite(LEDL, HIGH);
      digitalWrite(LEDR, HIGH);
      delay(50);
      digitalWrite(LEDL, LOW);
      digitalWrite(LEDR, LOW);
       
    }

    delay(50);

    if(digitalRead(B3) == LOW){

      ble.sendCommandCheckOK(F("AT+BLEHIDCONTROLKEY=MEDIANEXT"));
      draw(3);
      digitalWrite(LEDR, HIGH);
      delay(50);
      digitalWrite(LEDR, LOW);
       
    }

    delay(50);
      

}
