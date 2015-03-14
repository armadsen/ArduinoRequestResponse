This project is a simple Arduino sketch to be used as a companion to the [ORSSerialPort](http://www.github.com/armadsen/ORSSerialPort/) [RequestResponseDemo project](https://github.com/armadsen/ORSSerialPort/tree/master/Examples/RequestResponseDemo) for OS X. It can currently only be run on the [Arduino Esplora](http://arduino.cc/en/Main/ArduinoBoardEsplora).

The program simply listens on the Arduino's serial port at 57600 baud for incoming commands. The following command protocol is supported:

   Function      |  Command  |                      Response
-----------------|-----------|------------------------------------------------
Turn LED on      | `$LED1;`  | `!LED1;`
Turn LED off     | `$LED0;`  | `!LED0;`
Read LED         | `$LED?;`  | `!LED<0 or 1>;`
Read temperature | `$TEMP?;` | `!TEMP<x>;` (`<x>` = temperature in degrees C)

For more information, please see the ORSSerialPort [RequestResponseDemo project](https://github.com/armadsen/ORSSerialPort/tree/master/Examples/RequestResponseDemo) as well as the [Request Response API Documentation](https://github.com/armadsen/ORSSerialPort/wiki/Request-Response-API).

This project is licensed under the terms of the LGPL 3.0 [license](LICENSE.md). This project includes and uses [Packetizer](https://github.com/i-n-g-o/Packetizer) by Ingo Randolph under the terms of its [LGPL license](https://github.com/i-n-g-o/Packetizer/blob/master/license.txt).