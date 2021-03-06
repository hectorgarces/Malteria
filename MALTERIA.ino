#include <Wire.h>
#include <Adafruit_AM2315.h>
#include <SHT2x.h>
#include <bqLiquidCrystal.h>
#include <avr/wdt.h>
#include <EEPROM.h>

//Definicion de temperaturas limites
#define TEMP_MIN  30  //En grados Celsius
#define TEMP_MAX  32  //En grados Celsius

// Definicion de tiempos de calentamiento y enfriamiento
#define TIEMPO_CALENTAMIENTO  60L // En segundos y en formato LONG
#define TIEMPO_ENFRIAMIENTO  60L // En segundos y en formato LONG

#define LCD_I2C_ADD   0x20

#define DIRECCION_STATE_EEPROM  0

#define LED1  13
#define LED2  12

/*   Modulo de 4 Reles    
     Conexiones en Arduino:
     
     GND --> GND  
     VCC --> 5V  
     COM --> 5V (con alimentacion externa)
     
     RELE_1  --> D7  
     RELE_2  --> D6
     RELE_3  --> D5
     RELE_4  --> D4  */
#define RELAY1  7     // se definen los pines donde se conectaran los reles                
#define RELAY2  6                        
#define RELAY3  5                        
#define RELAY4  4

// Posibles estados del sistema
#define C1STOP_C2STOP  0x00
#define C1OFF_C2OFF  0x01
#define C1ON_C2OFF  0x02
#define C1OFF_C2ON  0x03
#define C1ON_C2ON  0x04

 /****************************************************/
// Connect RED of the AM2315 sensor to 5.0V
// Connect BLACK to Ground
// Connect WHITE to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect YELLOW to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
/****************************************************/

Adafruit_AM2315 am2315;
LiquidCrystal lcd(0); //creamos un objeto LCD por I2C, direccionado en 0x20 y a 100KHz

int temperatura1 = 0;
int temperatura2 = 0;
int calentador1 = 0;
int calentador2 = 0;

byte state;// = C1OFF_C2OFF;
//byte state_eeprom;

unsigned long timeout_calentamiento = 0; 
unsigned long timeout_enfriamiento = 0; 

unsigned long timeActual = 0;
unsigned long timeC1ON = 0;
unsigned long timeC1OFF = 0;
unsigned long timeC2ON = 0;
unsigned long timeC2OFF = 0;

void setup() 
{
  timeout_calentamiento = TIEMPO_CALENTAMIENTO * 1000; //en milisegundos
  timeout_enfriamiento = TIEMPO_ENFRIAMIENTO * 1000; //en milisegundos
  Serial.begin(9600);
  Serial.println("MALTERIA!");

  // Se configuran los pines como salidas
  pinMode(RELAY1, OUTPUT);      
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);

  // Se configuran los pines de los LEDs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT); 
  //Se encienden los LEDs
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  delay(2000);
  //Se apagan los LEDs
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  
  
  // INICIALIZACION DEL LCD POR I2C
  lcd.begin(16, 2); //inicializamos el LCD, 20 columnas, 4 filas
  lcd.clear(); //borramos cualquier contenido residual   
  lcd.setCursor(0,0); //ponemos el cursos en la columna 0, fila 0
  lcd.print("Malteria"); //imprimos este texto
  lcd.setCursor(0,1); //ponemos el cursos en la columna 0, fila 0
  lcd.print("Daniel Lozano"); //imprimos este texto

  Serial.print("AM2315");
  am2315.begin(); 

  attachInterrupt(0,botonStop,FALLING);
  
  // Se habilita el Watchdog a 8 segundos
  wdt_enable(WDTO_8S);
  
  state = EEPROM.read(DIRECCION_STATE_EEPROM);
  Serial.print("Sistema Iniciado ");

}// FIN del SETUP

// BUCLE PRINCIPAL
void loop() 
{
  // Se reinicia el WatchDog
  wdt_reset();
  Serial.print("Pre - Temperatura1: "); Serial.println(temperatura1);
  //Serial.print("Pre - Temperatura2: "); Serial.println(temperatura2);
  temperatura1 = (int)(am2315.readTemperature());
  //temperatura2 = (int)(SHT2x.GetTemperature());
  Serial.print("Temperatura1: "); Serial.println(temperatura1);
  //Serial.print("Temperatura2: "); Serial.println(temperatura2);

 timeActual = millis();
 
 Serial.print("STATE: "); Serial.println(state);
 
 switch(state)
 {
   //SISTEMA PARADO
   case C1STOP_C2STOP:
     fixState(C1STOP_C2STOP);
     break;
     
   //LOS DOS CALENTADORES APAGADOS
   case C1OFF_C2OFF:
     fixState(C1OFF_C2OFF);
     Serial.print("CASE C1OFF_C2OFF ");
     // Se arranca el calentador 1 y se almacena el timestamp para saber cuando se arranco el calentador 1
     if((int)temperatura1 < TEMP_MIN)
     {
       state = C1ON_C2OFF;
       fixState(C1ON_C2OFF);
       timeC1ON = millis();
     }
     else if( ((int)temperatura1 >= TEMP_MIN) && ((int)temperatura1 < TEMP_MAX) )
     {
        break;     
     }
     else if((int)temperatura1 >= TEMP_MAX)
     {
        break; 
     }
     break;
   
   // CALENTADOR1 = ON y CALENTADOR2 = OFF
  case C1ON_C2OFF:
    fixState(C1ON_C2OFF);
    Serial.print("CASE C1ON_C2OFF ");
    // Se comprueba la temperatura del sensor con la temperatura minima del malteador
    // Si el tiempo transucrrido desde que se arranco el calentador 1 es mayor al tiempo de calentamiento,
    // se arranca el calentador 2.
    // Se comprueba la temperatura del sensor con la temperatura maxima del malteador
    // Si se alcanza la temperarura maxima, se apaga el Calentador 1
    Serial.print("T_Actual: "); Serial.println(timeActual);
    Serial.print("T_C1ON: "); Serial.println(timeC1ON);
    Serial.print("T_Calent: "); Serial.println(timeout_calentamiento);
    
    // Si no se ha alcanzado la temperatura minima tras un tiempo de calentamiento
    // hay que anrrancar el calentador 2
    if((int)temperatura1 < TEMP_MIN)
    {
      // Se comprueba si se ha superado el tiempo de calentamiento desde el arranque del calentador 1
      // para arrancar el calentador 2
      if(timeActual > timeC1ON + timeout_calentamiento)
      {              
        state = C1ON_C2ON;
        fixState(C1ON_C2ON);      
        timeC2ON = millis();
      }
      else
        break;
     }
     
    else if( ((int)temperatura1 >= TEMP_MIN) && ((int)temperatura1 < TEMP_MAX) )
    {
      if(timeActual > timeC1ON + timeout_calentamiento)
      {              
        state = C1ON_C2ON;
        fixState(C1ON_C2ON);    
        timeC2ON = millis();  
      }
      else
        break;
    }   
   
   // Si se ha superado la temperatura maxima, se apaga el calentador 1
   else if((int)temperatura1 >= TEMP_MAX)
   {
     if(timeActual > timeC2OFF + timeout_enfriamiento)
     {
       state = C1OFF_C2OFF;
       fixState(C1OFF_C2OFF);    
       timeC1OFF = millis();
     }
    else 
     break; 
   } 
           
   break;
   
  case C1ON_C2ON:
    Serial.print("CASE C1ON_C2ON ");
    fixState(C1ON_C2ON);
    // Se comprueba la temperatura del sensor con la temperatura maxima del malteador
    // Si se alcanza la temperarura maxima, se apaga el Calentador 2
    if((int)temperatura1 < TEMP_MIN)
      break;
         
    else if( ((int)temperatura1 >= TEMP_MIN) && ((int)temperatura1 < TEMP_MAX) )
        break;
       
    else if((int)temperatura1 >= TEMP_MAX)
    {
      state = C1ON_C2OFF;
      fixState(C1ON_C2OFF);
      timeC2OFF = millis();      
    }  
    
   break;
   
  default:
   state = C1OFF_C2OFF;
   break;
  }// FIN del SWITCH
 
 delay(3000);
 lcd.clear();
 printTempLCD(temperatura1, temperatura2);
 printHeaterStatusLCD(calentador1, calentador2);

}// FIN del LOOP


void printTempLCD(int t1, int t2)
{
 String temp1 = "T1=" + String(t1) + "C"; //Se forma la cadena de texto de la temperatura T1
 String temp2 = "T2=" + String(t2) + "C"; //Se forma la cadena de texto de la temperatura T2

 String line = String(temp1 + " | " + temp2); // Se forma la cadena de la linea 1 del display

  //ejecutamos esto solo la primera vez
 //lcd.clearLine(1); //limpiamos cualquier contenido residual de la linea 1 del display

  lcd.setCursor(0,1); //ponemos el cursos en la columna 0, fila 1
  lcd.print(line); //imprimos este texto
}


void printHeaterStatusLCD(int C1State, int C2State)
{
  String c1state = "OFF";
  String c2state = "OFF"; 
  
  if(C1State==-1)
    c1state = "STOP";
  else if(C1State==0)
    c1state = "OFF";
  else if(C1State==1)
    c1state = "ON";
    
  if(C2State==-1)
    c2state = "STOP";
  else if(C2State==0)
    c2state = "OFF";
  else if(C2State==1)
    c2state = "ON";
 
  String line = "C1=" + String(c1state) + " | " + "C2=" + String(c2state);
  //lcd.clearLine(0); //limpiamos cualquier contenido residual de la linea 1 del display
  lcd.setCursor(0,0); //ponemos el cursos en la columna 0, fila 1
  lcd.print(line); //imprimos este texto
}

void fixState(byte systemState)
{
  switch (systemState)
  {
    case C1STOP_C2STOP:
      calentador1=-1;
      calentador2=-1;
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(RELAY1,LOW); 
      digitalWrite(RELAY2,LOW); 
      break;
    
    case C1OFF_C2OFF:
      calentador1=0;
      calentador2=0;
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(RELAY1,LOW); 
      digitalWrite(RELAY2,LOW); 
    break;
  
     case C1ON_C2OFF:
     calentador1=1;
     calentador2=0;
     digitalWrite(LED1, HIGH);
     digitalWrite(LED2, LOW);
     digitalWrite(RELAY1,HIGH); 
     digitalWrite(RELAY2,LOW); 
    break; 
  
     case C1OFF_C2ON:
     calentador1=0;
     calentador2=1;
     digitalWrite(LED1, LOW);
     digitalWrite(LED2, HIGH);
     digitalWrite(RELAY1,LOW); 
     digitalWrite(RELAY2,HIGH); 
    break; 
  
     case C1ON_C2ON:
     calentador1=1;
     calentador2=1;
     digitalWrite(LED1, HIGH);
     digitalWrite(LED2, HIGH);
     digitalWrite(RELAY1,HIGH); 
     digitalWrite(RELAY2,HIGH); 
    break;    
 
   default:
    break;   
 }// FIN del SWITCH  
 
 EEPROM.write(DIRECCION_STATE_EEPROM,systemState);
}


void botonStop() 
{    
  // Se deshabilitan la interrupción del boton STOP
  detachInterrupt(0);
     
  byte state_aux;  
  state_aux = EEPROM.read(DIRECCION_STATE_EEPROM);
  if(state_aux == C1STOP_C2STOP)
  {
    state = C1OFF_C2OFF;    
  }
  else
  {
    state = C1STOP_C2STOP;
  }
  //EEPROM.write(DIRECCION_STATE_EEPROM,state);
  fixState(state);
  // Se añade una espera para ignorar los rebotes del pulsador
  delay(1000);
  //printTempLCD(temperatura1, temperatura2);
  //printHeaterStatusLCD(calentador1, calentador2);
  // Se vuelve a habilitar la interrupción del boton STOP
  attachInterrupt(0,botonStop,FALLING);
}
