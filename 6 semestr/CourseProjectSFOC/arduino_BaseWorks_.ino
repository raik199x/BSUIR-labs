#include <Wire.h>       // I2C library
#include <microLED.h>   // LED library

#define STRIP_PIN 11     // pin for the diodes
#define NUMLEDS 16      // number of diodes
#define COLOR_DEBTH 1  // color depth of the diodes
#define BRIGHTNESS 60 // brightness of the diodes
microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip; //Initializing LED

const int P[] = {5, 4, 3, 2};     // rows
const int M[] = {9, 8, 7, 6};    // columns
const char k4x4 [4][4] = {      // keyboard symbols
  {'R', '6', 'I', 'A'},
  {'M', '1', '0', 'E'},
  {'U', '9', 'G', 'S'},
  {'D', 'F', 'N', 'B'}
};
int matrix[4][4];           // matrix for remembering pressed buttons
bool startArduino = false;  // flag for starting arduino
const int I2C_address = 21; // ESP32 address

// Function to receive data from ESP32 (basically receives only success from esp32)
void receiveEvent(int howMany) {
  char letter = '\0';
  while (Wire.available())
    letter = Wire.read();
  
  if(letter == 'S')
    startArduino = true;
}

// Since diodes connect differently, we need to determine which actual diode we need to light up
byte getPosDiode(char letter){
  char allLetters[] = {'B', 'A', 'S', 'E', '0', 'G', 'I', 'N', 'F', '6', '9', '1', 'M', 'U', 'R', 'D'};
  for(byte i = 0; i < 16; i++)
    if (allLetters[i] == letter)
      return i;
  Serial.println("WARNING: pressed letter was not found\n");
}

// Randoms color for diodes
mData randomColor(){
  // we end to 200 since all 255 is white color with is very greedy
  // we start from 20 so diodes wound be blank
  return mRGB(random(20,200), random(20,200), random(20,200));
}

// Makes diodes blink 3 times with setted color
void lightsBlink(mData color){
  for(byte i = 0; i < 3; i++){
    strip.clear();
    strip.show();
    delay(500);
    for(byte j = 0; j < NUMLEDS; j++)
      strip.set(j, color);
    strip.show();
    delay(500);
  }
}

// wait in running mode (yellow diode runs through all diodes)
void lightsWaitRunning(){
  for(byte i = 0; i < NUMLEDS; i++){
    receiveEvent(1);
    if(startArduino == true)
      return;
    strip.clear();
    strip.set(i, mRGB(255, 255, 0));
    strip.show();
    delay(600);
  }

  for(int i = NUMLEDS - 1; i >= 0; i--){
    receiveEvent(1);
    if(startArduino == true)
      return;
    strip.clear();
    strip.set(i, mRGB(255,255,0));
    strip.show();
    delay(600);
  }
}

void lightsWaitFilling(){
  strip.clear();
  delay(100);
  for(byte i = 0; i < NUMLEDS; i++){
    receiveEvent(1);
    if(startArduino == true)
      return;
    strip.set(i, mRGB(255, 255, 0));
    strip.show();
    delay(600);
  }
  delay(100);
  for(int i = NUMLEDS - 1; i >= 0; i--){
    receiveEvent(1);
    if(startArduino == true)
      return;
    strip.set(i, mRGB(0,0,0));
    strip.show();
    delay(500);
  }
}


void setup() {
  Wire.begin(18);                 // First, we connect as slave to i2c bus
  Wire.onReceive(receiveEvent);  // setting function for receiving data
  
  // keyboard set up
  for(byte i = 0; i < 4; i++)
    for(byte j = 0; j < 4; j++)
      matrix[i][j] = 0;
  for (byte i = 0; i < 4; i++) {  // making pin out rows - exit, columns - enter
    pinMode(P[i], OUTPUT);
    pinMode(M[i], INPUT_PULLUP);
    digitalWrite(P[i], HIGH);
  }

  Wire.setClock(400000);
  strip.setBrightness(BRIGHTNESS); // Setting up LED

  Serial.begin(9600);
  Serial.println("begin");

  // Waiting for ESP32 to start
  int waitingMode = 0;
  while(startArduino == false){
    if(waitingMode == 0){
      lightsWaitRunning();
      waitingMode = 1;
    }
    else if(waitingMode == 1){
      lightsWaitFilling();
      waitingMode = 0;
    }
  }
  Wire.end();                   // now we wont be slave
  Wire.begin(I2C_address);      // now we are master
  lightsBlink(mRGB(0, 255, 0)); // Showing user that set up is complete
  delay(500);
  strip.clear();
  strip.show();
}

void loop() {
  Wire.beginTransmission(I2C_address);
  for (byte p = 0; p < 4; p++) {    // placing LOW on rows
    digitalWrite(P[p], LOW);
    for (byte m = 0; m < 4; m++) {  // checking if row is actually LOW
      if (!digitalRead(M[m]) && matrix[p][m] == 0) {
        Serial.print("Pressed Button: ");
        Serial.print(k4x4[p][m]);
        Serial.print("\n");

        Wire.write(k4x4[p][m]);
        strip.set(getPosDiode(k4x4[p][m]), randomColor());
        strip.show();
        matrix[p][m] = 1;
      } else if (digitalRead(M[m]) && matrix[p][m] == 1){
        Serial.print("Unpressed Button: ");
        Serial.print(k4x4[p][m]);
        Serial.print("\n");

        strip.set(getPosDiode(k4x4[p][m]), mRGB(0,0,0));
        strip.show();
        matrix[p][m] = 0;
      }
    }
    digitalWrite(P[p], HIGH);       // return pin to HIGH
  }
  Wire.endTransmission();
}