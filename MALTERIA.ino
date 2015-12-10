#include <Wire.h>
#include <Adafruit_AM2315.h>
#include <SHT2x.h>
#include <bqLiquidCrystal.h>

#define LCD_I2C_ADD   0x20

#define LED1  13
#define LED2  12

/*   Modulo de 4 Reles    
     Conexiones en Arduino:
     
     GND --> GND  
     VCC --> 5V  
     COM --> 5V (con alimentacion externa)
     
     IN1  --> D11  
     IN2  --> D10
     IN3  --> D9
     IN4  --> D8  */
#define RELAY1  11     // se definen los pines donde se conectaran los reles                
#define RELAY2  10                        
#define RELAY3  9                        
#define RELAY4  8

// Posibles estados del sistema
#define C1OFF_C2OFF  0
#define C1ON_C2OFF  1
#define C1OFF_C2ON  2
#define C1ON_C2ON  3

//Definicion de temperaturas limites
#define TEMP_MIN  40  //En grados Celsius
#define TEMP_MAX  60  //En grados Celsius

// Definicion de tiempos de calentamiento y enfriamiento
#define TIEMPO_CALENTAMIENTO  60 // En segundos
#define TIEMPO_ENFRIAMIENTO  60 // En segundos

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

int state = C1OFF_C2OFF;

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
  Serial.println("AM2315 Test!");

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
  lcd.begin(20, 2); //inicializamos el LCD, 20 columnas, 4 filas
  lcd.clear(); //borramos cualquier contenido residual   
  lcd.setCursor(0,0); //ponemos el cursos en la columna 0, fila 0
  lcd.print("Malteria"); //imprimos este texto
  lcd.setCursor(0,1); //ponemos el cursos en la columna 0, fila 0
  lcd.print("Daniel Lozano"); //imprimos este texto

  while (! am2315.begin()) 
  {
     Serial.println("Sensor 1 not found, check wiring & pullups!");
     delay(1000);
  }

}// FIN del SETUP

// BUCLE PRINCIPAL
void loop() 
{
  temperatura1 = (int)(am2315.readTemperature());
  //temperatura2 = (int)(SHT2x.GetTemperature());
  Serial.print("Temperatura1: "); Serial.println(temperatura1);
  //Serial.print("Temperatura2: "); Serial.println(temperatura2);
 

 timeActual = millis();
 
 switch(state)
 {
  //LOS DOS CALENTADORES APAGADOS
  case C1OFF_C2OFF:
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
      state = C1OFF_C2OFF;
      fixState(C1OFF_C2OFF);    
      timeC1OFF = millis(); 
  } 
           
   break;
   
  case C1ON_C2ON:
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
   break;
  }// FIN del SWITCH
 
 lcd.clear();
 printTempLCD(temperatura1, temperatura2);
 printHeaterStatusLCD(calentador1, calentador2);

 delay(5000);
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
  
  if(C1State)
    c1state = "ON";
  if(C2State)
    c2state = "ON";
 
  String line = "C1=" + String(c1state) + " | " + "C2=" + String(c2state);
  //lcd.clearLine(0); //limpiamos cualquier contenido residual de la linea 1 del display
  lcd.setCursor(0,0); //ponemos el cursos en la columna 0, fila 1
  lcd.print(line); //imprimos este texto
}

void fixState(int systemState)
{
  switch (systemState)
 {
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
 }  
}