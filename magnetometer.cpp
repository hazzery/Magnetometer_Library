#include "magnetometer.h"
#include <Arduino.h>
#include <EEPROM.h>
#include "Wire.h"

Magnetometer::Magnetometer(byte address = 0x0d)
:addr(address){}

void Magnetometer::init()//Initialize magnetometer.
{
  Wire.begin();//Start I2C communication for magnetometer.

  writeToRegistery(0x0b, 1);//Tell the magnetometer to run in continuous mode.
  writeToRegistery(0x09, B00000001); 
}

void Magnetometer::writeToRegistery(byte registryAddr, byte dataByte)//Send data to specified register.
{
  Wire.beginTransmission(addr);//Begin communication with magnetometer.
  Wire.write(registryAddr);//Tell magnetometer the register we want to write to.
  Wire.write(dataByte);//Write data to megnertometer register.
  Wire.endTransmission();//End communication with magnetometer.
}

bool Magnetometer::ready()//Is the magnetometer ready?
{
  Wire.beginTransmission(addr);//Begin communication with magnetometer.
  Wire.write(0x06);//Tell magnetometer we want to read from register 6.
  Wire.requestFrom(addr, (byte)1);//Tell magnetometer we want 1 byte.
  byte readData = Wire.read() & (byte)1;//Read the data from the megnetometer.
  Wire.endTransmission();//End communication with magnetometer.
  return readData;//Return the data we read.
}

void Magnetometer::readRawData()//Gets raw xyz data from magnetometer.
{ 
  if (!ready())
    return;

  Wire.beginTransmission(addr);//Begin communication with magnetometer.
  Wire.write(0x00);//Tell magnetometer to start reading from register zero.
  Wire.endTransmission();//End communication with magnetometer.
  
  Wire.requestFrom(addr, (byte)6);//Tell megnetometer we want 6 bytes.

  x = (int16_t)(Wire.read() | Wire.read() << 8);//Save x ordinate.
  y = (int16_t)(Wire.read() | Wire.read() << 8);//Save y ordinate.
  z = (int16_t)(Wire.read() | Wire.read() << 8);//Save z ordinate.
}

void Magnetometer::calculateOffsets()
{
  offsetX = (maxX + minX) / 2;
  offsetY = (maxY + minY) / 2;
}

void Magnetometer::calibrate()
{
  const int calibrationLength = 10000;//Length of calibration - in ms.
  const int eepromAddress = 66;//Start address for EEPROM.

  unsigned long startTime = millis();//Time at start of calibration
  
  bool calibrating = 1;
  byte startCal = 1;

  while (!ready());
	readRawData();

  maxX = minX = x;//Set initial values to current magnetometer readings.
  maxY = minY = y;

  while(calibrating)
  {
    if (ready())
    {
      readRawData();

      if (x > maxX) maxX = x;
      else if (x < minX) minX = x;
      if (y > maxY) maxY = y;
      else if (y < minY) minY = y;
    }
      
  
    int timeDifference  = millis() - startTime;
    int secs = (int)((calibrationLength - timeDifference + 1000) / 1000);
    Serial.print("Calibrating, ");
    Serial.print((calibrationLength - timeDifference) / 1000);
    Serial.println(" secconds remaining.");
  
    if (secs == 0)//Calibration has finished.
    {
      calculateOffsets();
      calibrating = 0;

      int EEAddr = eepromAddress;
      EEPROM.put(EEAddr, minX);
      EEAddr += sizeof(minX);
      EEPROM.put(EEAddr, maxX);
      EEAddr += sizeof(maxX);
      EEPROM.put(EEAddr, minY);
      EEAddr += sizeof(minY);
      EEPROM.put(EEAddr, maxY);
      EEAddr += sizeof(maxY);
      
      Serial.println("EEPROM Written");
    }
  
    delay(10);
  } //while(calibrating)
}

int Magnetometer::getX()
{ return x; }
int Magnetometer::getY()
{ return y; }
int Magnetometer::getZ()
{ return z; }

int Magnetometer::getBearing()
{
  int atan2val = 180/M_PI * atan2((float)(x - offsetX),(float)(y - offsetY));
  bearing = (-atan2val + 360 ) % 360;

  return bearing;
}