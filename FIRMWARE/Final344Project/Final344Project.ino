/*
   LED Matrix Etch-A-Sketch
   Mariano Elia
*/
#include "LedControl.h" //Matrix driver for MAX7219 chip
#include <TimerOne.h> //Used for interrupt service routine

/*
   Define the pins for the I/O used
*/
const int XDirPotPin = A5;
const int YDirPotPin = A4;
const int XDirAccelPin = A2;
const int YDirAccelPin = A1;
const int ZDirAccelPin = A0;
const int pushButtonPin = A3;
const int MAX_DEVICES = 4;
const int CLK_PIN = 7;
const int DATA_PIN = 5;
const int CS_PIN = 6;

byte currBuffer[32] = {}; //Holds the current buffer which is yet to be written
byte prevBuffer[32] = {}; //Holds the previous buffer that was written to the display

//Setup the LED Control library with the panel name 'lc', as well as initiate the SPI bus
//as it was chosen to use software defined bus for easier Printed Circuit Board layout
LedControl lc = LedControl(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES); //Setup the SPI Bus pinout

void setup() {
  //Potentiometer and Accelerometer inputs
  pinMode(XDirPotPin, INPUT);
  pinMode(YDirPotPin, INPUT);
  pinMode(XDirAccelPin, INPUT);
  pinMode(YDirAccelPin, INPUT);
  pinMode(ZDirAccelPin, INPUT);

  int devices = lc.getDeviceCount();
  //we have to init all devices in a loop
  for (int address = 0; address < devices; address++) {
    //The MAX72XX is in power-saving mode on startup
    lc.shutdown(address, false);
    // Set the brightness to a medium values
    lc.setIntensity(address, 8);
    // and clear the display
    lc.clearDisplay(address);
  }
  Serial.begin(115200); // 115200 Baud Rate
  clearDisplay(); // Initialize all buffers


  /*
     Run an automated routine every 4 milliseconds to check potentiometers and accelerometers.
     This uses another of the hardware timers in the ATMEGA328 to automatically call the function
     'checkSensors' regardless if there is a delay() being currently executed
  */
  Timer1.initialize(4000);
  Timer1.attachInterrupt(checkSensors);
} //End void setup()

void loop() {
  updateDisplay(); // update every 30 milliseconds
  delay(30);
} //End void loop()

void clearDisplay() {
  for (int i = 0; i < 32; i++) { //Erase both buffers
    currBuffer[i] = 0b00000000;
    prevBuffer[i] = 0b00000000;
  }
  //And write to the display
  int devices = lc.getDeviceCount();
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      for (int address = 0; address < devices; address++) {
        lc.setLed(address, row, col, false);
      }
    }
  }
}


//Add a pixel to the currBuffer
void drawPixel(int X, int Y) {
  if (X < 0 || Y < 0 || X < 32 || Y < 8 ) //Check it is in bounds of matrix
    currBuffer[X] |= (1 << Y); //Bit shift a 1 to the correct (X,Y)
}



void updateDisplay() {
  int devices = lc.getDeviceCount();
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      for (int address = 0; address < devices; address++) {
        int hi;        
        //Each panel is 8x8, and in total there are 4 panels
        //making it 8x32
        if (address == 0) { //most left panel
          // lo = 0;
          hi = 7;
        }
        else if (address == 1) {
          // lo = 8;
          hi = 15;
        }
        else if (address == 2) {
          // lo = 16;
          hi = 23;
        }
        else { //address == 3 //most right panel
          //  lo = 24;
          hi = 31;
        }
        /*
           This next section was added in to optimize writing to the display
           as it is a time consuming task. The last written byte(column of the
           display) is compared to the current buffer's byte, and only if they have
           changed will it be written.
        */
        byte previousByte = prevBuffer[hi - col];
        byte currentByte = currBuffer[hi - col];
        
        //If the previous byte is different at that particular row
        if ((currentByte >> row) != (previousByte >> row)) { 
          prevBuffer[hi - col] |= (1 >> row);
          if ((1 & (currentByte >> row)) == 1) {
            lc.setLed(address, row, col, true); //Write a pixel as a 1
          }
        }
      }
    }
  }
}


void checkSensors() {

  int xPot = analogRead(XDirPotPin);
  int yPot = analogRead(YDirPotPin);

  int xAccel = analogRead(XDirAccelPin);
  int yAccel = analogRead(YDirAccelPin);
  int zAccel = analogRead(ZDirAccelPin);
  int buttonState = digitalRead(pushButtonPin);
  /*
     If the button is pressed, or any of the accelerometer axis
     are over a threshold of 500 (2.44 Volts), the display will be cleared
  */
  if (xAccel > 500 || yAccel > 500 || zAccel > 500 || buttonState == HIGH) {
    clearDisplay();
    Serial.println("CLEARING DISPLAY!!");
  }

  //Print Potentiometer Data
  /*
  Serial.print("X Pot: ");
  Serial.print(xPot);
  Serial.print(" Y Pot: ");
  Serial.println(yPot);
 */
  /*
     Print Accelerometer Data. It is commented out as it is slows down the
     program to run many print statements every 4 milliseconds.
  */

  /*
    Serial.print(" X Accel: ");
    Serial.print(xAccel);
    Serial.print(" Y Accel: ");
    Serial.print(yAccel);
    Serial.print(" Z Accel: ");
    Serial.println(zAccel);
  */

  xPot = map(xPot, 0, 1023, 0, 32); //Map the column
  yPot = map(yPot, 1023, 0, 0, 7); //Map the row
  drawPixel(xPot, yPot);
}

