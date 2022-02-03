/*
 Arduino Power Meter

 This sketch works with power sensing chips, such as the Analog
 Devices AD8318 (1mhz to 8GHz, 55 dB range up to 10dBm) and an LCD display.
 It reads the analog output value from the sensor, converts the voltage
 to it's dBm representation, and displays the values on an LCD screeen
 
 The voltage is also printed to the serial port, and can be graphed with
 the Arduino IDE's "Serial Plotter" tool.

 Glen Overby, KC0IYT

 The LCD display code is based on the LiquidCrystal Library - Serial Input

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch displays text sent over the serial port
 (e.g. from the Serial Monitor) on an attached LCD.

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://arduino.cc/en/Tutorial/LiquidCrystalSerial

 Developed using a KXM2004A 20x4 LCD display
 Connections to LCD: 
    rs = 7
    enable = 8
    d4 = 9
    d5 = 10
    d6 = 11
    d7 = 12
    
 Connection to calibration selection button: 2
 Connection to power sensor: A0
 */

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

#define BUTTON 2

struct cal {
  char *id;
  int slope;   // Positive (1) or negative (-1) slope
  int dbmv;    // db per millivolt
  int bottomdb; // lowest db reading at lowest voltage
  double bottomv; // lowest voltage
};

// Calibration table
struct cal calibration[] = {
  { "8318", -1, 25, -57, 2.0 },
  { "1G", -1, 22, -60, 1.62 },
  { "10G", -1, 22, -60, 1.62 },
  { "144", -1, 22, -60, 1.62 },
};

char cal_selected = 0;   // selected calibration set
char button = 1;  // button state

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // initialize the serial communications:
  Serial.begin(9600);
  // Initialize the calibration switch button
  pinMode(BUTTON, INPUT_PULLUP);
}


void loop() {
  int a; // analog value
  int n; // number of bars
  double v; // voltage value
  double dv; // voltage relative to calibration
  double dB; // calculated dBm value
  double dbmv; // db per millivolt
  char str[32]; // formatted string for output
  
  a = analogRead(0);
  v = (5.0 / 1024.0) * a;
  //Serial.print(a);
  //Serial.print(" ");
  //Serial.println(v);  // Print for Arduino chart

  // Print to display: 
  // Row 1: Voltage, dB and calibration selection
  // Row 2: Bar Graph

  lcd.clear();
  // Print voltage
  //dtostrf(v, 6, 4, str);
  //lcd.write(str);
  //lcd.write("v ");

  // Convert voltage based on the sensor's slope and minimum value
  // For example, the AD8318 outputs 2.0v at ~-57dBm and 0.6v at 
  // ~2dBm. This is a negative slope.
  if (calibration[cal_selected].slope > 0) {
    dv = v - calibration[cal_selected].bottomv;
  } else {
    dv = calibration[cal_selected].bottomv - v;
  }
  dbmv = calibration[cal_selected].dbmv / 1000.0;
  dB = calibration[cal_selected].bottomdb + dv / dbmv;

  // Print dB for Arduino chart
  Serial.println(dB);
  
  // Print dB
  dtostrf(dB, 6, 2, str);
  lcd.write(str);
  lcd.write(" dBm ");

  // Print which calibration is selected (the ID string in the struct)
  lcd.write(" ");
  lcd.write(calibration[cal_selected].id);

  lcd.setCursor(0,2);
  n = abs(calibration[cal_selected].bottomdb - dB) / 3.5;
    // 70dBm range, 20 characters 
  //n = a/51;  // 20 character display: 1024 / 20 = 51
  while (n-- > 0) {
    lcd.write(255); // character is a full block
  }

  // Read the calibration change button. If pressed, debounce
  // then advance the selected calibration, wrapping around at
  // the end.
  if (button != digitalRead(BUTTON)) {
    delay(10); // debounce the button
    button = digitalRead(BUTTON);
    if (button == 0) {
      cal_selected++; // next item
      if (cal_selected >= sizeof(calibration)/sizeof(struct cal)) {
        cal_selected = 0; // return to zero at the end
      }
    }
  }
  delay(100);
}
// vim: tabstop=2 expandtab shiftwidth=2 softtabstop=2
