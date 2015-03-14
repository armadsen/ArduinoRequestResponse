#include <Esplora.h>
#include "Packetizer.h"

Packetizer slicer;

void setup() {
  Serial.begin(57600);
  configurePacketizer();
}

void configurePacketizer()
{
  slicer.init(128);
  slicer.setStartCondition("$");
  slicer.setEndCondition(";");
  slicer.onPacket(serialPacketWasReceived);  
}

void loop() {
  readSerialData();
  
  // Toggle LED on/off if down button is pressed.
  if (buttonIsDown()) {
    isLEDOn() ? Esplora.writeRGB(0, 0, 0) : Esplora.writeRGB(255, 255, 255);
  }
}

void readSerialData() 
{
  while (Serial.available()) {
    int inputByte = Serial.read();    
    if (inputByte < 0) continue;
    
    slicer.appendData((uint8_t)inputByte);
  }
}

// Debounces button
bool buttonIsDown(void) 
{
  // Used for button debounce logic
  static int buttonState;             // the current reading from the button
  static int lastButtonState = LOW;   // the previous reading from the button
  static long lastDebounceTime = 0;  // the last time the output pin was toggled
  static long debounceDelay = 50;    // the debounce time; increase if the output flickers
  
  int newState = Esplora.readButton(SWITCH_DOWN);
  
  if (newState != lastButtonState) {
    lastDebounceTime = millis(); // reset debounce timer
  }
  lastButtonState = newState;
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // newState is now considered valid
    if (newState != buttonState) {
      buttonState = newState;
      if (buttonState == LOW) return true;
    }
  }
  
  return false;
}

void serialPacketWasReceived(byte* inputData, unsigned int inputSize)
{
  if (!memcmp(inputData, "LED", min(inputSize, 3))) {
    handleLEDCommand(inputData, inputSize);
    return;
  }
  
  if (!memcmp(inputData, "TEMP?", min(inputSize, 5))) {
    sendTemperature();
    return;
  }
}

// Utilities

bool isLEDOn(void)
{
  return (Esplora.readRed() | Esplora.readGreen() | Esplora.readBlue()) != 0;
}

// Command handlers

void handleLEDCommand(byte *inputData, unsigned int inputSize)
{
  if (inputSize < 4) return; // Invalid command
  byte commandByte = inputData[3];
  bool ledIsOn = false;
  switch (commandByte) {
    case '?':
      ledIsOn = isLEDOn();
      break;
    case '0':
      Esplora.writeRGB(0, 0, 0);
      ledIsOn = false;
      break;
    case '1':
      Esplora.writeRGB(255, 255, 255);
      ledIsOn = true;
      break;
    default:
      // Invalid command
      return;
      break;
  }
  
  ledIsOn ? Serial.print("!LED1;") : Serial.print("!LED0;");
}

void sendTemperature(void)
{
  int celsius = Esplora.readTemperature(DEGREES_C);
  Serial.print("!TEMP");
  Serial.print(celsius);
  Serial.print(";");
}


