// Hay que usar la libreria bqLiquidCrystal

#include <bqLiquidCrystal.h>
#include <Wire.h>
 
/***   Global variables   ***/
LiquidCrystal lcd(0); //creamos un objeto LCD
 
void setup()
{
  lcd.begin(20, 2); //inicializamos el LCD, 20 columnas, 4 filas
  lcd.clear(); //borramos cualquier contenido residual
 
}
 
 
void loop()
{
//ejecutamos esto solo la primera vez
lcd.clear(); //limpiamos cualquier contenido residual
lcd.setCursor(0,0); //ponemos el cursos en la columna 0, fila 0
lcd.print("T1=20C T2=30C"); //imprimos este texto
lcd.setCursor(0,1); //ponemos el cursos en la columna 0, fila 0
lcd.print("T3=40C T4=50C"); //imprimos este texto

delay(5000);
 
}
