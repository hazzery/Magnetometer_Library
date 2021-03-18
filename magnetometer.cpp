#include "magnetometer.h"
#include <Arduino.h>
#include "Wire.h"

Magnetometer::Magnetometer(byte address = 0x0d)
:addr(address){}

void Magnetometer::init()
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

bool Magnetometer::magReady()//Is the magnetometer ready?
{
  Wire.beginTransmission(addr);//Begin communication with magnetometer.
  Wire.write(0x06);//Tell magnetometer we want to read from register 6.
  Wire.requestFrom(addr, (byte)1);//Tell magnetometer we want 1 byte.
  byte readData = Wire.read() & (byte)1;//Read the data from the megnetometer.
  Wire.endTransmission();//End communication with magnetometer.
  return readData;//Return the data we read.
}

int* Magnetometer::getRawData()
{
  static int coords[3];
  
  Wire.beginTransmission(addr);//Begin communication with magnetometer.
  Wire.write(0x00);//Tell magnetometer to start reading from register zero.
  Wire.endTransmission();//End communication with magnetometer.
  
  Wire.requestFrom(addr, (byte)6);//Tell megnetometer we want 6 bytes.
  coords[0] = (int16_t)(Wire.read() | Wire.read() << 8);//Get X data.
  coords[1]  = (int16_t)(Wire.read() | Wire.read() << 8);//Get Y data.
  coords[2]  = (int16_t)(Wire.read() | Wire.read() << 8);//Get Z data.

  return coords;
}
