#include <Wire.h>
#include <bqLiquidCrystal.h>
#include <Adafruit_AM2315.h>

#define LCD_I2C_ADD   0x20
#define NO_AM2315_SENSOR  1
#define LED1  13

// Connect RED of the AM2315 sensor to 5.0V
// Connect BLACK to Ground
// Connect WHITE to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect YELLOW to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
Adafruit_AM2315 am2315;

int temperatura1=0;
int temperatura2=0;
int pinState = 0;
unsigned long repeticion = 0;
unsigned long error = 0;

LiquidCrystal lcd(0); //creamos un objeto LCD por I2C, direccionado en 0x20 y a 100KHz

void setup() 
{
  Serial.begin(9600);
  Serial.println("LCD_I2C Test!");
  
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, HIGH);
  pinState = 1;
  
  while(! am2315.begin()) 
  {
    Serial.println("Sensor not found, check wiring & pullups!");
    delay(1000);
  }
  
  // INICIALIZACION DEL LCD POR I2C
  lcd.begin(20, 2); //inicializamos el LCD, 20 columnas, 4 filas
  lcd.clear(); //borramos cualquier contenido residual   
  lcd.setCursor(0,0); //ponemos el cursos en la columna 0, fila 0
  lcd.print("Malteria"); //imprimos este texto
  lcd.setCursor(0,1); //ponemos el cursos en la columna 0, fila 0
  lcd.print("Daniel Lozano"); //imprimos este texto
  delay(5000);
}

void loop() 
{
  // Se cambia el estado del LED1
  if(pinState == 1)
  {
    digitalWrite(LED1, LOW);
    pinState = 0;
  }
  else if(pinState == 0)
  {
    digitalWrite(LED1, HIGH);
    pinState = 1;
  }
      
  temperatura1 = (int)(am2315.readTemperature());
  delay(5000);
  temperatura2 = (int)(am2315.readHumidity());
  //Serial.print("Hum: "); Serial.println(temperatura2);
  //Serial.print("Temp: "); Serial.println(temperatura1);
 
  if( (temperatura1 == 0) && (temperatura2 == 0))
  {
    errorManager(NO_AM2315_SENSOR);
    error++;
  }
   else
  { 
    printTempLCD(temperatura1, temperatura2, repeticion, error);
    repeticion++;
  }
  delay(5000);
}



void printTempLCD(int t1, int t2, unsigned long rep, unsigned long err)
{
  String cabecera = "R:" + String(rep) + " | E:" + String(err);
    
  String temp1 = "T:" + String(t1) + "C"; //Se forma la cadena de texto de la temperatura T1
  String temp2 = "H:" + String(t2) + "%"; //Se forma la cadena de texto de la temperatura T2

  String line = String(temp1 + " | " + temp2); // Se forma la cadena de la linea 1 del display

  lcd.clear(); //limpiamos cualquier contenido residual del display
  
  lcd.setCursor(0,0); //ponemos el cursos en la columna 0, fila 0
  lcd.print(cabecera); //imprimos este texto

  //ejecutamos esto solo la primera vez
  lcd.setCursor(0,1); //ponemos el cursos en la columna 0, fila 1
  lcd.print(line); //imprimos este texto
}

int errorManager (int errorCode)
{
 
  switch (errorCode)
  {
   case NO_AM2315_SENSOR:
     lcd.clear(); //limpiamos cualquier contenido residual del display  
     lcd.setCursor(0,0); //ponemos el cursos en la columna 0, fila 0
     lcd.print("ERROR LECTURA"); //imprimos este texto
     lcd.setCursor(0,1); //ponemos el cursos en la columna 0, fila 0
     lcd.print("SENSOR AM2315"); //imprimos este texto
    
     // release i2c bus
     //TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT);

     /*digitalWrite(SDA, 0);
     digitalWrite(SCL, 0);   
     digitalWrite(SDA, 1);
     digitalWrite(SCL, 1);
     am2315.begin();*/
     break;
   
   
   default:
     break;
  } 
  
}
