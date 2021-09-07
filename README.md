# rpi4-max1160x
A testing example for MAX1160x analog-to-digital converter for RPi4

# Description

MAX1160x is a family of multichannel ADCs (analog to digital converters) controlled via I2C interface. This simple software is used to config and read data from the MAX11602 ADC (currently). Other ICs from the MAX1160x family are not supported yet.

# Platform info
This code is being tested on the following configuration:

Platform:   Raspberry Pi 4B+
OS:         Linux ubuntu
OS version: 5.4.0-1044-raspi

# Dependencies

1. WiringPi (gpio) version 2.52 or higher. The official WiringPi library is unsupported, the active fork can be found here: https://github.com/WiringPi/WiringPi
2. To run the program, first the I2C kernel module needs to be loaded. This can be done using the GPIO utility.
   > gpio load i2c 400
   
   In my case it was not necessary.

# Notes

This code has been created basing on https://github.com/AllAboutEE/MAX11609EEE-Breakout-Board library for MAX11609, and also on this article: https://learn.sparkfun.com/tutorials/raspberry-pi-spi-and-i2c-tutorial/all.
The documentation for WiringPi I2C library can be found here: http://wiringpi.com/reference/i2c-library/

The init call returns a standard file descriptor. More detailed configuration of the interface can be performed using ioctl calls on that descriptor. See the wiringPi I2C implementation (wiringPi/wiringPiI2C.c) for some examples.
Parameters configurable with ioctl are documented here:
https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/Documentation/i2c/dev-interface.rst

# Build

To build the executable code, enter the following command:
> g++ max116xx-read.cpp -l wiringPi -o max116xx-read

# Usage

Run the program
> sudo ./max1160x-read
or, to refresh the values on screen every 0,5s:
> watch -n 1 sudo ./max1160x-read
