#pragma once

#include <Arduino.h>

class Magnetometer
{
  private:
    byte addr;
    int x, maxX, minX, offsetX;
    int y, maxY, minY, offsetY;
    int z, maxZ, minZ, offsetZ;
    
  public:
    Magnetometer(byte address);
    
    void init();
    void writeToRegistery(byte registryAddr, byte dataByte);
    bool ready();
    void readRawData();
    void calculateOffsets();
    void calibrate();
    
    int getX();
    int getY();
    int getZ();

    int getBearing();
};
