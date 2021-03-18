//Gets raw data from magnetometer(QMC5883L) and prints to serial moniter.
#include <magnetometer.h>

Magnetometer mag(0x0d);//Create a new magnetometer object at I2C address 0x0d.

void setup()
{
  Serial.begin(9600);//Begin serial communication with computer.
  mag.init();//Initialize the magnetometer.
}

void loop()
{
  int *coords = mag.getRawData();//Get pointer to array containing coordinates.
  
  //Name each ordinate as seperate variables.
  int x = coords[0];
  int y = coords[1];
  int z = coords[2];
  
  //Prints each ordinate to serial moniter.
  Serial.print("X: ");
  Serial.println(x);
  Serial.print("Y: ");
  Serial.println(y);
  Serial.print("Z: ");
  Serial.println(z);

  delay(100);
}
