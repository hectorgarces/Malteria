#include <Wire.h>
#include <bqLiquidCrystal.h>

#define LCD_I2C_ADD   0x20

int temp1=0;
int temp2=10;
int temp3=20;
int temp4=30;

LiquidCrystal lcd(0); //creamos un objeto LCD por I2C, direccionado en 0x20 y a 100KHz

void setup() 
{
  Serial.begin(9600);
  Serial.println("LCD_I2C Test!");
  
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
 //Serial.print("Hum: "); Serial.println(am2315.readHumidity());
 //Serial.print("Temp: "); Serial.println(am2315.readTemperature());
 printTempLCD(temp1, temp2, temp3, temp4);
 
 temp1++;
 temp2++;
 temp3++;
 temp4++;
 
 delay(1000);
}


void printTempLCD(int t1, int t2, int t3, int t4)
{
 String temp1 = "T1=" + String(t1); //Se forma la cadena de texto de la temperatura T1
 String temp2 = "T2=" + String(t2); //Se forma la cadena de texto de la temperatura T2
 String temp3 = "T3=" + String(t3); //Se forma la cadena de texto de la temperatura T3
 String temp4 = "T4=" + String(t4); //Se forma la cadena de texto de la temperatura T4
 
 String line1 = String(temp1 + " | " + temp2); // Se forma la cadena de la linea 1 del display
 String line2 = String(temp3 + " | " + temp4); // Se forma la cadena de la linea 2 del display
 
  //ejecutamos esto solo la primera vez
 lcd.clear(); //limpiamos cualquier contenido residual

 lcd.setCursor(0,0); //ponemos el cursos en la columna 0, fila 0
 lcd.print(line1); //imprimos este texto
 lcd.setCursor(0,1); //ponemos el cursos en la columna 0, fila 0
 lcd.print(line2); //imprimos este texto
}
