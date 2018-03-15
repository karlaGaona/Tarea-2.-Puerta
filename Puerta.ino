/*
----------------------------------------------------------------------
Interruptor magnético - Control de acceso de una puerta
-----------------------------------------------------------------------
Programa que genera una alarma con una Pantalla LCD y un Buzzer para 
emitir un mensaje de alerta y un sonido, a partir de un campo magnético generado a un
reed switch, logrando disminuir el consumo de energía.

Sistemas Programables
Karla Paola Gaona Delgado
15 de marzo del 2018
*/

//--------------------------------------------------
//Declara librerías a utilizar
//--------------------------------------------------
#include <LiquidCrystal.h>    // Incluye la Libreria LiquidCrystal para utilizar la Pantalla LCD
#include <avr/sleep.h>        // Incluye la Librería AVR/SLEEP para susperder el arduino

//--------------------------------------------------
//Declara puertos de entradas y salidas y variables
//--------------------------------------------------
int contacto = 8;                         // Pin asignado al reed switch
int buzzer = 9;                           // Pin asignado al Buzzer
LiquidCrystal lcd(1, 2, 4, 5, 6, 7);      // Crea un Objeto LC. Parametros: (rs, enable, d4, d5, d6, d7)
int duracion=250;                         // Duración del sonido
int fMin=2000;                            // Frecuencia más baja que queremos emitir
int fMax=4000;                            // Frecuencia más alta que queremos emitir
int i=0;
int LED = 13;                             // Parpadeo antes de dormir
int pinDespertar = 3;                     // pin utilizado para la interrupción
int contador = 0;
int tiempo = 10;                          // Variable para el tiempo que durará el sonido de la alarma

// ---------------------------------------------------------------------
// Método para reproducir el sonido de la alarma mediante las variables de frecuencia más baja y más alta,
// reproduciendose durante un tiempo determinado
// ----------------------------------------------------------------------
void sonido(){
  for(contador;contador<=tiempo;contador++){
    //sonido más agudo
    for (i=fMin;i<=fMax; i++)
      tone(buzzer, i, duracion);
    //sonido más grave
    for (i=fMax;i>=fMin; i--)
      tone(buzzer, i, duracion);
  }
  contador=0;
}

//------------------------------------
// Método para manejar la interrupción
//------------------------------------
void despertar() {  
  // ejecuta el código aquí después de la activación antes de volver al ciclo () los temporizadores de la función y el 
  // código que usa temporizadores (serial.print y más ...) no funcionarán aquí.
  // en realidad no necesitamos ejecutar ninguna función especial aquí, ya que solo queremos que se despierte  
}

//------------------------------------
// Método para dormir el arduino, contribuyendo a disminuir el consumo de energía
//------------------------------------
void dormir() {  
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // Se establece el modo de bajo consumo  
    sleep_enable();                         // inicializa el sistema para ponerse en modo bajo consumo pero aun no esta activo  
    attachInterrupt(0,despertar, LOW);      // usa la interrupción 0 (pin 2) y ejecuta la función. LOW: la interrupción se dispara cuando el pin está en estado LOW.
    sleep_mode();                           // activa el modo bajo consumo 
    // EL PROGRAMA CONTINÚA DESDE AQUÍ DESPUÉS DE DESPERTAR 
    sleep_disable();                        // primera instrucción que debe aparecer cuando se vuelve del modo bajo consumo. desactiva el modo bajo consumo  
    detachInterrupt(0);                     // Deshabilita la interrupción 0 en el pin 2 por lo que el código despertar no se ejecutará durante el tiempo de ejecución normal.
}

//------------------------------------
//Funcion principal
//------------------------------------
void setup() {// Se ejecuta cada vez que el Arduino se inicia
  pinMode(pinDespertar, INPUT);
  pinMode(LED, OUTPUT);
  attachInterrupt(0, despertar, LOW);   // usa la interrupción 0 (pin 2) y ejecuta la función wakeUpNow cuando el pin 2 obtiene LOW
  pinMode(contacto,INPUT);              //El reed switch como una entrada
  pinMode (buzzer, OUTPUT);             //pin configurado como salida
  lcd.begin(16,2);                      // Inicializa la interface para el LCD screen, and determina sus dimensiones (ancho y alto) del display                
  lcd.clear();                          // Limpia la pantalla                  
}

//------------------------------------
//Funcion ciclicla
//------------------------------------
void loop() // Esta funcion se mantiene ejecutando
{ // cuando este energizado el Arduino
  digitalWrite(LED, HIGH);  
  //delay(1000);  
  digitalWrite(LED, LOW);    
  
  // Si el iman esta lejos del reed switch se activa la alarma
  if (digitalRead(contacto)==HIGH){
    lcd.clear();                          // Limpia la pantalla  
    lcd.setCursor(0,0);                   // setea el cursor a la columna 0, fila 0
    lcd.print("Control Acceso");          // ingresamos nuestro texto
    lcd.setCursor(1,1);                   // setea el cursor a la columna 1, fila 1
    lcd.print("Alerta Intruso!!!");       // imprime mensaje de alerta
    sonido();
    delay(50);                            // setea el tiempo del desplazamiento del caracter
  // Si el iman esta cerca del reed switch se desactiva la alarma
  }else{
    lcd.clear();                          // Limpia la pantalla  
    lcd.setCursor(0,0);                   // setea el cursor a la columna 0, fila 0
    lcd.print("Control Acceso");          // ingresamos nuestro texto
    lcd.setCursor(1,1);                   // setea el cursor a la columna 1, fila 1
    lcd.print("Activado");                // imprime mensaje
    delay(50);                            // setea el tiempo del desplazamiento del caracter
  } //Fin del if

  dormir();     // Se llama a la función de dormir
}
