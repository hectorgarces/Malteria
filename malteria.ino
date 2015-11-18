#include <Wire.h>
#include <Adafruit_AM2315.h>
#include <LiquidCrystal_I2C.h>



#define BACKLIGHT_PIN     13
#define LCD_LINES     2
#define LCD_CHARTS    16
#define LCD_I2C_ADD   0x27

#define ID_I2C_TEMP1  0x11
#define ID_I2C_TEMP2  0x12
#define ID_I2C_TEMP3  0x13
#define ID_I2C_TEMP4  0x14

 /****************************************************/
// Connect RED of the AM2315 sensor to 5.0V
// Connect BLACK to Ground
// Connect WHITE to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect YELLOW to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
/****************************************************/

Adafruit_AM2315 Sensor1, Sensor2, Sensor3, Sensor4;
LiquidCrystal_I2C lcd(LCD_I2C_ADD, LCD_CHARTS, LCD_LINES);  // Set the LCD I2C address

void setup() 
{
  Serial.begin(9600);
  Serial.println("AM2315 Test!");
  
  // INICIALIZACION DEL LCD POR I2C
  lcd.init();               // initialize the lcd 
  lcd.clear();
  lcd.home ();                   // go home
  lcd.print("Hello, ARDUINO ");  
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print (" WORLD!  ");     

  // INICIALIZACION DE LOS SENSORES
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
