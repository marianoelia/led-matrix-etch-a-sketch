/*
 * ELEC344 Final Project
 * Mariano Elia and Janelle Green
 */
 
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

 
const int XDirPotPin = A0;
const int YDirPotPin = A1;
const int ZDirPotPin = A2;

const uint16_t WAIT_TIME = 1000;


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
  pinMode(ZDirPotPin, INPUT);

  P.begin(); //Start the LED panel
}

void loop() {
  // put your main code here, to run repeatedly:


  
  
}

void clearDisplay(){

}
void drawPixel(int X, int Y){

}



