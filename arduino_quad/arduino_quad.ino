/* ============================================
Quadcopter arduino code
This will read from serial (written to by the pi), and
write via pwm the states of all the motors as fast as 
possible. 
Modified from i2cdev example by Ryan Gaus

I2Cdev device library code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// and, math too
#include "math.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for our board)
// AD0 high = 0x69
MPU6050 accelgyro;

// store motion information
int16_t ax, ay, az;
int16_t gx, gy, gz;
int rotX, rotY, rotZ, Apwm, Bpwm, Cpwm, Dpwm;

// for status updates
#define LED_PIN 13
bool blinkState = false;

// offsets to keep vehicle level
#define XLEVELOFFSET 360
#define YLEVELOFFSET -200
#define ZLEVELOFFSET 200

// == motors ==
// (Front)
//    A
//  D   B
//    C
#define MOTORA 5
#define MOTORB 10
#define MOTORC 11
#define MOTORD 9

// z offset
float z_lift = 10;

// == angular offsets in degrees ==

// positive angular offset in X:
// (C)
//    \
//     \
//      (A)
float x_angular_offset = 0;

// positive angular offset in Y:
// (D)
//    \
//     \
//      (B)
float y_angular_offset = 0;

// incoming serial data
char incomingAxis;
short incomingValue = 0;

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #endif

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(9600);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // configure pins
    pinMode(LED_PIN, OUTPUT);
    pinMode(MOTORA, OUTPUT);
}

void loop() {
  
  
  
  
    // watch for serial input
    if (Serial.available() >= 4) {
      incomingValue = 0;
      
      // read the incoming bytes
      for (int i = 0; i < 4; i++) {
        if (i == 0) {
          incomingAxis = Serial.read();
        } else {
          //temp = (Serial.read());
          //Serial.println( (temp-48) * pow(10, 3-i) );
          incomingValue += (((Serial.read())-48) * pow(10, 3-i));
        }
      }
      
      // add one, because we are in the wrong base (1 vs 0)
      incomingValue++;
      
      // make sure we are within bounds
      while (incomingValue > 360) { incomingValue -= 360; };
      while (incomingValue < 0) { incomingValue += 360; };

      // say what you got:
      //Serial.print(incomingAxis);
      //Serial.print(' ');
      //Serial.println(incomingValue);
      
      
      // x and y rotational axes (0 to 360 => 180 to -180)
      if (incomingAxis == 'x') {
        x_angular_offset = 180-incomingValue;
      } else if (incomingAxis == 'y') {
        y_angular_offset = 180-incomingValue;
      
      // z is a little different - relimit the values passed in and
      // then set it. (0 to 255)
      } else if (incomingAxis == 'z') {
        while (incomingValue > 255) { incomingValue -= 255; };
        while (incomingValue < 0) { incomingValue += 255; };
        z_lift = incomingValue;
      }
    }
  
  
  
  
  
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    // calculate rotation on each axis
    rotX = (180/3.141592) * atan(ax / sqrt(square(ay) + square(az))); 
    rotY = (180/3.141592) * atan(ay / sqrt(square(ax) + square(az)));
    rotZ = (180/3.141592) * atan(sqrt(square(ay) + square(ax)) / az);

    // display tab-separated accel/gyro x/y/z values
    //Serial.print("a/g:\t");
    //Serial.print(ax); Serial.print("\t");
    //Serial.print(ay); Serial.print("\t");
    //Serial.print(az); Serial.print("\t");
    //Serial.print(rotX);
    //Serial.print("\t");
    //Serial.print(rotY);
    //Serial.print("\t");
    //Serial.println(rotZ);

    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
    
    // === calculate motor values ===
    // simply put, at rotX or rotY moves from -90 to 90, 
    // each pwm moves from 0 to 255 for each axis, or 255 to 0
    // if the axis is inverted
    // = MOTORA_PERCX*(255-(((rotX+90)/180.0)*255)) + MOTORA_PERCY*(255-(((rotY+90)/180.0)*255));
    
    // A: -X
    Apwm = (((-1*rotX+x_angular_offset)/90.0)*255)+z_lift;
    if (Apwm < 0) Apwm = 0;
    
    // C: +X
    Cpwm = (((rotX-x_angular_offset)/90.0)*255)+z_lift;
    if (Cpwm < 0) Cpwm = 0;
    
    
    
    // B: +Y
    Bpwm = (((-1*rotY+y_angular_offset)/90.0)*255)+z_lift;
    if (Bpwm < 0) Bpwm = 0;
    
    // D: +Y
    Dpwm = (((rotY-y_angular_offset)/90.0)*255)+z_lift;
    if (Dpwm < 0) Dpwm = 0;
    
    
    
    // write to motors
    /*
    Serial.print(Apwm); Serial.print("\t");
    Serial.print(Cpwm); Serial.print("\t");
    Serial.println();
    //*/
    analogWrite(MOTORA, Apwm);
    analogWrite(MOTORB, Bpwm);
    analogWrite(MOTORC, Cpwm);
    analogWrite(MOTORD, Dpwm);
}
