# PowerMeter
Arduino Based RF Power Meter

This is a power meter that uses Analog Devices logarithmic detectors
to detect the RF power, an Arduino, and an LCD display. It currently
supports the AD8318 sensor, with un-tested suport for the AD8317.

* The power is displayed in dBm
* Multiple power sensor profiles are supported.
Selection of a profile uses a momentary push button to go through the
profiles in circular order.
* The power value is printed to the serial port for use with the
Arduino IDE's Serial Plotter tool

This was developed with a KXM2004A 20x4 LCD display, but only two lines
are currently used. It should work with a 16x2 display, with minor
modifications. Using different LCD display connections will require
changing the LiquidCrystal library initialization.

Glen Overby  2/2/2022

![Image of board wired up](https://raw.githubusercontent.com/glenoverby/PowerMeter/main/images/v1.jpg)



