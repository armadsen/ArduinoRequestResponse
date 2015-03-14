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
}

void readSerialData() 
{
  while (Serial.available()) {
    int inputByte = Serial.read();    
    if (inputByte < 0) continue;
    
    slicer.appendData((uint8_t)inputByte);
  }
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

// Command handlers

void handleLEDCommand(byte *inputData, unsigned int inputSize)
{
  if (inputSize < 4) return; // Invalid command
  byte commandByte = inputData[3];
  bool ledIsOn = false;
  switch (commandByte) {
    case '?':
      ledIsOn = (Esplora.readRed() | Esplora.readGreen() | Esplora.readBlue()) != 0;
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


