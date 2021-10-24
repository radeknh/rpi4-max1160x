/******************************************************************************
max1160x-read.cpp
Raspberry Pi 4 - test of MAX11602 read
******************************************************************************/

#include <iostream>
#include <errno.h>
#include <wiringPiI2C.h>

using namespace std;

#define MAX116xx_ADR    0x6d  // MAX11602 - 0x6d
#define RESOLUTION      8     // number of bits
// configuration byte - for single channel (ch0) conversion, single-ended mode
#define CONFIG          0b01100001  
// setup byte - ref = VDD, internal clock, unipolar mode
#define SETUP           0b10000010

uint16_t readSingleCh(int fd, uint8_t channel)
{
   uint16_t result = 0x0000;
   
   //single ended mode, convert a single channel
   uint8_t configurationByte = ( (channel<<1) & 0b00011110) | 0b01100001; 
   wiringPiI2CWrite(fd, configurationByte);

#if RESOLUTION > 8
   // for more than 8-bit MAX116xx
   result = (wiringPiI2CRead(fd) & 0x03)<<8; // MSB is returned first. [7-2] are high.
   result |= wiringPiI2CRead(fd) & 0x00FF; // read LSB
#elseif
   result = wiringPiI2CRead(fd) & 0xFFFF;   
#endif
   return result;
}

/*void readNChTest(int n, int &fd)
{
   
}*/

int main()
{
   int fd, result;

   // Initialize the interface by giving it an external device ID instead of I2C device address.
   // It returns a standard file descriptor.
   fd = wiringPiI2CSetup(MAX116xx_ADR);
   if(fd < 0) cout << "I2C initialization error. Errno is: " << errno << endl;

   wiringPiI2CWrite(fd, CONFIG & 0b01111111); // MSB=0 chooses a configuration byte (7 bits)
   wiringPiI2CWrite(fd, SETUP | 0b10000000); // MSB=1 chooses a setup byte (7 bits)
   
   // configuration byte - for single channel conversion, single-ended mode
   config(0b01100001, &fd);
   // setup byte - ref = VDD, internal clock, unipolar mode
   setup(0b10000010, &fd);

   result = readSingleCh(fd, 0);

   if(result == -1)
   {
      cout << "Error.  Errno is: " << errno << endl;
   }else{
      cout << "Result: " << result << endl;
   }
}
