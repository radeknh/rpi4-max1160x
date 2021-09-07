/******************************************************************************
max1160x-read.cpp
Raspberry Pi 4 - test of MAX11602 read
******************************************************************************/

#include <iostream>
#include <errno.h>
#include "wiringPiI2Cmod.h"
#include <wiringPi.h>
#include <bitset>
#include <unistd.h>

using namespace std;

#define MAX116xx_ADR    0x33  // MAX11602 - 0x6d, MAX11609 - 0x33
#define RESOLUTION      10     // number of bits
// configuration byte - for single channel (ch0) conversion, single-ended mode
#define CONFIG          0b01100001  
// setup byte - ref = VDD, internal clock, unipolar mode
#define SETUP           0b10000010
#define VREF 3.2 // volts
#define CONFIGALL	0b00001111 //scan 8 channels



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

int main(int argc, const char* argv[])
{
   int fd, result = 0;
   bool commOk = 0;
   float voltage[8] = {0};
   uint16_t buf[33] = {0};

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
   // read the all channels in one I2C command - the last I2C uint16_t is written first to buf
   wiringPiI2CWrite(fd, CONFIGALL);
   result = read(fd, buf+16, 16);
   
   for(int i=0; i<33; i++)
   {
      std::cout << "Result all " << std::bitset<16>(buf[i]) 
                << " " << buf[i] << endl;
   }
   /*
   for(int i=0; i<8 && commOk; i++)
   {
      result = readSingleCh(fd, i);
      std::cout << "Result CH" << i << " " << std::bitset<16>(result) 
                << " " << result << endl;
      voltage[i] = (VREF*result)/1023;
      cout << "Result in volts: " << voltage[i] << "V" << endl;
   }*/
}


