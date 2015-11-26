#include <Wire.h>
#include <Adafruit_AM2315.h>
#include <bqLiquidCrystal.h>

#define LCD_I2C_ADD   0x20

#define ID_I2C_TEMP1  0x11
#define ID_I2C_TEMP2  0x12

 /****************************************************/
// Connect RED of the AM2315 sensor to 5.0V
// Connect BLACK to Ground
// Connect WHITE to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect YELLOW to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
/****************************************************/

Adafruit_AM2315 Sensor1, Sensor2;
LiquidCrystal lcd(0); //creamos un objeto LCD por I2C, direccionado en 0x20 y a 100KHz

int temperatura1 = 0;
int temperatura2 = 0;
int calentador1 = 0;
int calentador2 = 0;

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
}

void loop() 
{

 //Serial.print("Hum: "); Serial.println(am2315.readHumidity());
 //Serial.print("Temp: "); Serial.println(am2315.readTemperature());
 temperatura1 = (int)(am2315.readTemperature());
 temperatura2 = (int)(sht21.readTemperature());
 
 switch(state)
 {
  case OFF:
   // Se arranca el calentador 1 y se almacena el timestamp para saber cuando se arranco el calentador 1
   break;
   
  case C1ON_C2OFF:
   // Se comprueba la temperatura del sensor con la temperatura minima del malteador
   // Si el tiempo transucrrido desde que se arranco el calentador 1 es mayor al tiempo de calentamiento,
   // se arranca el calentador 2.
   
    // Se comprueba la temperatura del sensor con la temperatura maxima del malteador
    // Si se alcanza la temperarura maxima, se apaga el Calentador 1
   break;
   
  case C1ON_C2ON:
    // Se comprueba la temperatura del sensor con la temperatura maxima del malteador
    // Si se alcanza la temperarura maxima, se apaga el Calentador 2
   break;
   
  case default:
   break;
  
  
  
 }
 
 
 printTempLCD(temperatura1, temperatura2);
 printHeaterStatusLCD(calentador1, calentador2);
 
 t1++;
 t2++;
 delay(1000);
}


void printTempLCD(int t1, int t2)
{
 String temp1 = String("T1=" + t1); //Se forma la cadena de texto de la temperatura T1
 String temp2 = String("T2=" + t2); //Se forma la cadena de texto de la temperatura T2

 String line = String(temp1 + " | " + temp2); // Se forma la cadena de la linea 1 del display

  //ejecutamos esto solo la primera vez
 lcd.clearLine(1); //limpiamos cualquier contenido residual de la linea 1 del display

 lcd.setCursor(0,1); //ponemos el cursos en la columna 0, fila 1
 lcd.print(line); //imprimos este texto
}

void printHeaterStatusLCD(int H1State, int H1State)
{
 String h1tate = "OFF";
 String h2state = "OFF";
 
 if(H1State)
 {
  h1tate = "ON";
 }
  if(H2State)
 {
  h2tate = "ON";
 }
 
 String line = String("C1=" + h1tate + " | " + "C2=" + h2tate);
 lcd.clearLine(0); //limpiamos cualquier contenido residual de la linea 1 del display
 lcd.setCursor(0,0); //ponemos el cursos en la columna 0, fila 1
 lcd.print(line); //imprimos este texto
}
}
