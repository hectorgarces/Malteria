#include <Wire.h>
#include <bqLiquidCrystal.h>
#include <Adafruit_AM2315.h>
#include <avr/wdt.h>
#include <EEPROM.h>

#define DIRECCION_ERROR_EEPROM  0
#define DIRECCION_EJECUCION_INICIAL_EEPROM  1
#define DIRECCION_LCD_I2C  0
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
byte error = 0;
byte ejecucion_inicial = 0;
LiquidCrystal lcd(DIRECCION_LCD_I2C); //creamos un objeto LCD por I2C, direccionado en 0x20 y a 100KHz


void setup() 
{
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, HIGH);
  delay(500);
  digitalWrite(LED1, LOW);
  delay(500);
  digitalWrite(LED1, HIGH);
  delay(500);
  pinState = 1;
 
  
  Serial.begin(9600);
  Serial.println("Malteria. Daniel Lozano!");
  
  //LiquidCrystal lcd(0); //creamos un objeto LCD por I2C, direccionado en 0x20 y a 100KHz
 
  lcd.begin(16, 2); //inicializamos el LCD, 16 columnas, 2 filas
  lcd.clear(); //borramos cualquier contenido residual   
  
  // Cada vez que se ejecuta el codigo desde el principio, se lee el valor de error de la EEPROM, se incrementa y se vuelve a almacenar
  error = EEPROM.read(DIRECCION_ERROR_EEPROM);
  error++;
  EEPROM.write(DIRECCION_ERROR_EEPROM,error);  
  Serial.print("Error: "); Serial.println(error);
  
  // Se obtiene el valor del flag de EJECUCION_INICIAL
  ejecucion_inicial = EEPROM.read(DIRECCION_EJECUCION_INICIAL_EEPROM);
  
  // Se habilita el Watchdog a 8 segundos
  wdt_enable(WDTO_8S);
  lcd.clear(); //borramos cualquier contenido residual   
  lcd.setCursor(0,0); //ponemos el cursos en la columna 0, fila 0
  lcd.print("WatchDog:OK"); //imprimos este texto
  delay(1000);
  
  // Se habilita la interrupcion por el switch conectado al PIN 2
  attachInterrupt(0,BotonStop,FALLING);
  lcd.clear(); //borramos cualquier contenido residual   
  lcd.setCursor(0,0); //ponemos el cursos en la columna 0, fila 0
  lcd.print("Boton Stop:OK"); //imprimos este texto
  delay(1000);
 
  
  // INICIALIZACION DEL SENSOR DE TEMPERATURA AM2315 POR I2C
  am2315.begin(); 
  lcd.clear(); //borramos cualquier contenido residual   
  lcd.setCursor(0,0); //ponemos el cursos en la columna 0, fila 0
  lcd.print("AM2315:OK"); //imprimos este texto
  delay(1000);
  
  // INICIALIZACION DEL LCD POR I2C
  lcd.begin(20, 2); //inicializamos el LCD, 20 columnas, 4 filas
  lcd.clear(); //borramos cualquier contenido residual   
  lcd.setCursor(0,0); //ponemos el cursos en la columna 0, fila 0
  lcd.print("Malteria"); //imprimos este texto
  lcd.setCursor(0,1); //ponemos el cursos en la columna 0, fila 0
  lcd.print("Daniel Lozano"); //imprimos este texto
  delay(1000);
}


/****************** BUCLE PRINCIPAL *********************/
void loop() 
{
  // Se reinicia el WatchDog
  wdt_reset();
  
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
      
  // Se lee la temperatura y la humedad relativa
  temperatura1 = (int)(am2315.readTemperature());
  temperatura2 = (int)(am2315.readHumidity());
  //Serial.print("Hum: "); Serial.println(temperatura2);
  //Serial.print("Temp: "); Serial.println(temperatura1);
  delay(3000);
 
  if( (temperatura1 == 0) && (temperatura2 == 0))
  {
    errorManager(NO_AM2315_SENSOR);
    //error++;
  }
   else
  { 
    printTempLCD(temperatura1, temperatura2, repeticion, error);
    repeticion++;
  }
  delay(3000);
}
/**********************************************************/


void printTempLCD(int t1, int t2, unsigned long rep, byte err)
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

void BotonStop() 
{    
  //delay(2000);
  //Serial.println("Interrupcion en Pin 2!");
  // Se reinicia el valor de la cantidad de errores a 0
  error = 0;
  EEPROM.write(DIRECCION_ERROR_EEPROM,error);
  ejecucion_inicial = 0;
  EEPROM.write(DIRECCION_EJECUCION_INICIAL_EEPROM,ejecucion_inicial);
 
  //lcd.clear(); //borramos cualquier contenido residual   
  //lcd.setCursor(0,0); //ponemos el cursos en la columna 0, fila 0
  //lcd.print("Sistema Parado"); //imprimos este texto
  //lcd.setCursor(0,1); //ponemos el cursos en la columna 0, fila 0
  //lcd.print("Apague Sistema"); //imprimos este texto
  //Serial.println("_Interrupcion en Pin 2!");
}
