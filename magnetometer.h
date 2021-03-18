#include <Arduino.h>

class Magnetometer
{
  private:
    byte addr;
    
  public:
    Magnetometer(byte address);

    void init();
    void writeToRegistery(byte registryAddr, byte dataByte);
    bool magReady();
    int* getRawData();
};
