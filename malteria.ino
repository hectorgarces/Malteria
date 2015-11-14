#include <Wire.h>
#include <Adafruit_AM2315.h>

#define ID_I2C_TEMP1  0x11
#define ID_I2C_TEMP2  0x12
#define ID_I2C_TEMP3  0x13
#define ID_I2C_TEMP4  0x14

/*************************************************** 
  This is an example for the AM2315 Humidity + Temp sensor
  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> https://www.adafruit.com/products/1293
  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/
// Connect RED of the AM2315 sensor to 5.0V
// Connect BLACK to Ground
// Connect WHITE to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect YELLOW to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4

Adafruit_AM2315 Sensor1, Sensor2, Sensor3, Sensor4;

void setup() 
{
  Serial.begin(9600);
  Serial.println("AM2315 Test!");

  Sensor1.setI2cAdd(ID_I2C_TEMP1);
  Sensor1.setI2cAdd(ID_I2C_TEMP2);
  Sensor1.setI2cAdd(ID_I2C_TEMP3);
  Sensor1.setI2cAdd(ID_I2C_TEMP4);

  if (! Sensor1.begin()) 
  {
     Serial.println("Sensor 1 not found, check wiring & pullups!");
     while (1);
  }
  
    if (! Sensor2.begin()) 
  {
     Serial.println("Sensor 2 not found, check wiring & pullups!");
     while (1);
  }
  
    if (! Sensor3.begin()) 
  {
     Serial.println("Sensor 3 not found, check wiring & pullups!");
     while (1);
  }
  
    if (! Sensor4.begin()) 
  {
     Serial.println("Sensor 4 not found, check wiring & pullups!");
     while (1);
  }
  
}

void loop() {
  Serial.print("Hum: "); Serial.println(am2315.readHumidity());
  Serial.print("Temp: "); Serial.println(am2315.readTemperature());

  delay(1000);
}
