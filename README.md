The Quad
===

So, this is a project build my me, [Ryan Gaus](//github.com/1egoman) and [Sam
Stone](//github.com/sciencesam). It's a quadcoptor with a pi and arduino working side-by-side to do all
the processing. The arduino is conencted to an mpu-6050 gyroscope and will
communicate with the pi (over i2c, spi, or serial) to control the motions. The
Raspberry pi a+ runs some node/coffeescript that serves a web interface over the
network that allows a user to hopefully control the quad over wifi or for it to
do autonomous stuff.

```
                 I2C
  PI <== Arduino <== Gyro/sensors
            ||
            \/ PWM
           Motors
```
