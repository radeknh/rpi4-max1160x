/******************************************************************************
max1160x-read.cpp
Raspberry Pi 4 - test of MAX11602 read
******************************************************************************/

#include <iostream>
#include <errno.h>
#include <wiringPiI2C.h>
#include <bitset>

using namespace std;

#define MAX116xx_ADR    0x33  // MAX11602 - 0x6d, MAX11609 - 0x33
#define RESOLUTION      10     // number of bits
// configuration byte - for single channel (ch0) conversion, single-ended mode
#define CONFIG          0b01101001  
// setup byte - ref = VDD, internal clock, unipolar mode
#define SETUP           0b10000010



uint16_t readSingleCh(int fd, uint8_t channel)
{
   uint16_t result = 0x0000;
   uint8_t high_byte, low_byte;
   //single ended mode, convert a single channel
   uint8_t configurationByte = ( (channel<<1) & 0b00011110) | 0b01100001; 

#if RESOLUTION > 8  // for more than 8-bit MAX116xx
   result = wiringPiI2CReadReg16(fd,configurationByte); // high-byte is returned first. [7-2] are high.
   // switch high byte with low byte and remove leading ones
   low_byte = (result>>8) & 0xFF;
   high_byte = result & 0x03;
   result = high_byte<<8 | low_byte;
#else
   wiringPiI2CWrite(fd, configurationByte);
   result = wiringPiI2CRead(fd);   
#endif
   return result;
}

int main()
{
   int fd, result = 0;
   bool commOk = 0;
   // struct timespec remaining, request = {3, 500};

   // Initialize the interface by giving it an external device ID instead of I2C device address.
   // It returns a standard file descriptor.
   fd = wiringPiI2CSetup(MAX116xx_ADR);
   if(fd < 0) cout << "I2C initialization error. Errno is: " << errno << endl;

   if(wiringPiI2CWrite(fd, CONFIG & 0b01111111) < 0) 
   {
      cout << "ADC device comm error" << endl; // MSB=0 chooses a configuration byte (7 bits)
   }else{
      commOk = 1;
      wiringPiI2CWrite(fd, SETUP | 0b10000000); // MSB=1 chooses a setup byte (7 bits)
   }
   
   for(int i=0; i<1 && commOk; i++)
   {
      result = readSingleCh(fd, 4);
      //result = wiringPiI2CReadReg16(fd,CONFIG);
      //cout << "Result CH" << i << " " << result << endl;
      std::cout << "Result CH" << i << " " << std::bitset<16>(result) 
                << " " << result << endl;
   }
}
