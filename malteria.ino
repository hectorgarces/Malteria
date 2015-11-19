#include <Wire.h>
#include <Adafruit_AM2315.h>
#include <bqLiquidCrystal.h>

#define LCD_I2C_ADD   0x20

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
LiquidCrystal lcd(0); //creamos un objeto LCD por I2C, direccionado en 0x20 y a 100KHz

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

void loop() 
{
 int t1=0;
 int t2=10;
 int t3=20;
 int t4=30;
 
 //Serial.print("Hum: "); Serial.println(am2315.readHumidity());
 //Serial.print("Temp: "); Serial.println(am2315.readTemperature());
 printTempLCD(t1, t2, t3, t4);
 
 t1++;
 t2++;
 t3++;
 t4++;
 
 delay(1000);
}


void printTempLCD(int t1, int t2, int t3, int t4)
{
 String temp1 = String("T1=" + t1); //Se forma la cadena de texto de la temperatura T1
 String temp2 = String("T2=" + t2); //Se forma la cadena de texto de la temperatura T2
 String temp3 = String("T3=" + t3); //Se forma la cadena de texto de la temperatura T3
 String temp4 = String("T4=" + t4); //Se forma la cadena de texto de la temperatura T4
 
 String line1 = String(temp1 + " | " + temp2); // Se forma la cadena de la linea 1 del display
 String line2 = String(temp3 + " | " + temp4); // Se forma la cadena de la linea 2 del display
 
  //ejecutamos esto solo la primera vez
 lcd.clear(); //limpiamos cualquier contenido residual

 lcd.setCursor(0,0); //ponemos el cursos en la columna 0, fila 0
 lcd.print(line1); //imprimos este texto
 lcd.setCursor(0,1); //ponemos el cursos en la columna 0, fila 0
 lcd.print(line2); //imprimos este texto
}
