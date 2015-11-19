#include <Wire.h>
#include <Adafruit_AM2315.h>
#include <bqLiquidCrystal.h>


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
LiquidCrystal lcd(0); //creamos un objeto LCD 

void setup() 
{
  Serial.begin(9600);
  Serial.println("AM2315 Test!");
  
  // INICIALIZACION DEL LCD POR I2C
  lcd.begin(20, 2); //inicializamos el LCD, 20 columnas, 4 filas
  lcd.clear(); //borramos cualquier contenido residual   
  lcd.setCursor(0,0); //ponemos el cursos en la columna 0, fila 0
  lcd.print("Malteria"); //imprimos este texto
  lcd.setCursor(0,1); //ponemos el cursos en la columna 0, fila 0
  lcd.print("Daniel Lozano"); //imprimos este texto

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






 
 
void printTempLCD(int t1, int t2, int t3, int t4)
{
 
 String temp1 = "T1=" + t1; 
 String temp2 = "T2=" + t2;
 String temp3 = "T3=" + t3;
 String temp4 = "T4=" + t4;
 
 String line1 = temp1 + " " + temp2;
 String line2 = temp3 + " " + temp4;
 
//ejecutamos esto solo la primera vez
lcd.clear(); //limpiamos cualquier contenido residual

lcd.setCursor(0,0); //ponemos el cursos en la columna 0, fila 0
lcd.print(line1); //imprimos este texto
lcd.setCursor(0,1); //ponemos el cursos en la columna 0, fila 0
lcd.print(line2); //imprimos este texto

}
