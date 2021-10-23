/******************************************************************************
i2ctest.cpp
Raspberry Pi 4 - test of MAX11602 read

This example makes use of the Wiring Pi library
The I2C API is documented here:
https://projects.drogon.net/raspberry-pi/wiringpi/i2c-library/
******************************************************************************/

#include <iostream>
#include <errno.h>
#include <wiringPiI2C.h>

using namespace std;

#define MAX11602_ADR 0x6d

int main()
{
   int fd, setup_byte, config_byte, result;

   // Initialize the interface by giving it an external device ID instead of I2C device address.
   // It returns a standard file descriptor.
   fd = wiringPiI2CSetup(MAX11602_ADR);
   if(fd < 0) cout << "I2C initialization error. Errno is: " << errno << endl;

   // configuration byte - for single channel conversion, single-ended mode
   wiringPiI2CWrite(fd, 0b01100001);
   // setup byte - ref = VDD, internal clock, unipolar mode
   wiringPiI2CWrite(fd, 0b10000010);

   for(int i = 0; i < 1; i++)
   {
      result = wiringPiI2CRead(fd);

      if(result == -1)
      {
         cout << "Error.  Errno is: " << errno << endl;
      }else{
         cout << "Result: " << result << endl;
      }
   }
}
