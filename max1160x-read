/******************************************************************************
i2ctest.cpp
Raspberry Pi 4 - test of MAX11602 read
https://github.com/sparkfun/Pi_Wedge

A brief demonstration of the Raspberry Pi I2C interface, using the SparkFun
Pi Wedge breakout board.

This example makes use of the Wiring Pi library
The I2C API is documented here:
https://projects.drogon.net/raspberry-pi/wiringpi/i2c-library/

The init call returns a standard file descriptor.  More detailed configuration
of the interface can be performed using ioctl calls on that descriptor.
See the wiringPi I2C implementation (wiringPi/wiringPiI2C.c) for some examples.
Parameters configurable with ioctl are documented here:
http://git.kernel.org/cgit/linux/kernel/git/torvalds/linux.git/tree/Documentation/i2c/dev-interface

Hardware connections:
I2C 1 to MAX11602

To build this file, I use the command:
>  g++ i2ctest.cpp -lwiringPi

Then to run it, first the I2C kernel module needs to be loaded.  This can be
done using the GPIO utility.
> gpio load i2c 400
> ./a.out
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
