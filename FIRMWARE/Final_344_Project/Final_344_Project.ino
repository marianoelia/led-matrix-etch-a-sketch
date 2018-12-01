/*
 * ELEC344 Final Project
 * Mariano Elia and Janelle Green
 */
 
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

 
const int XDirPotPin = A5;
const int YDirPotPin = A4;
const int XDirAccelPin = A2;
const int YDirAccelPin = A1;
const int ZDirAccelPin = A0;
const int pushButtonPin = A3;
const uint16_t WAIT_TIME = 1000;

int currBuffer[32] = {};

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   7
#define DATA_PIN  5
#define CS_PIN    6

 MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);



void setup() {
  // put your setup code here, to run once:
  pinMode(XDirPotPin, INPUT);
  pinMode(YDirPotPin, INPUT);
  pinMode(XDirAccelPin, INPUT);
  pinMode(YDirAccelPin, INPUT);
  pinMode(ZDirAccelPin, INPUT);
  Serial.begin(115200);
  clearDisplay();
  P.begin(); //Start the LED panel
}

void loop() {
  // put your main code here, to run repeatedly:

//P.print(currBuffer);

int xval = map(analogRead(XDirPotPin), 0, 1023, 0, 32);
int yval = map(analogRead(YDirPotPin), 1023, 0, 0, 8);

Serial.print("X Direction val: ");
Serial.println(xval);
Serial.print("Y Direction val: ");
Serial.println(yval);
delay(100);

}

void clearDisplay(){
for(int i = 0; i<32; i++){
  currBuffer[i] = 0b00000000;
}
}

void drawPixel(int X, int Y){

}


//Use currBuffer to update display values
void updateDisplay(){
  
}








