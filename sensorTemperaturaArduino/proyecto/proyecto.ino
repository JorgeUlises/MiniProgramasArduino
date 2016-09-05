/*
  LiquidCrystal Library - Hello World
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World!" to the LCD
 and shows the time.
 
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 
 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
int buttonPin = 7;
int opt = 2;
int buttonState = 0;
int cambio = 0;

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Temperatura: ");
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);   
}

void loop() {
  sensorValue = analogRead(sensorPin);
  float valor = sensorValue;  
  float sense = (valor/1024)*5;
  String texto = "";
  if(opt == 0){
    sense = 25*sense-12.5;
    String temp = conv(sense);
    texto = temp + " " + (char)223 + "C" + "       ";
  } else if(opt == 1) {
    sense = 45*sense+9.5;
    String temp = conv(sense);
    texto = temp + " " + (char)223 + "F" + "       ";
  } else {
    sense = 25*sense+260.65;
    String temp = conv(sense);
    texto = temp + " K" + "       ";
  }
  Serial.println(sensorValue);
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  lcd.print(texto);
  modo();
  delay(100);
}

void modo(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {  
      cambio = 1;
  }
  else {
    if (cambio == 1){
      cambio = 0;
      opt = opt + 1;
      if(opt==3){
        opt=0;
      }
    }
  }
}

String conv(float number){
  char buf[32]; // needs to be at least large enough to fit the formatted text
  dtostrf(number, 4, 3, buf); 
  String temp = buf;
  return temp;
}

