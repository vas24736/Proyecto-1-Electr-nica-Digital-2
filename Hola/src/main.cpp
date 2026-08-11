//Universidad del Valle de Guatemala
//ELECTRÓNICA DIGITAL 2 - SECCIÓN - 10
//Israel Marcelino Vasquez Morales
//Proyecto 1

//librerias
#include <Arduino.h>
#include <stdio.h>
#include <stdint.h>
#include "driver/ledc.h"
#include "config.h"

//definiicon de funciones
void configB1(void);
void configPWM(void);
void configTimer(void);
void configDispley(uint8_t numero, uint8_t display, uint8_t punto);
void ledServo(float term);
void obtenerCifras(float t);
void IRAM_ATTR Timer0_ISR1();
void IRAM_ATTR Timer0_ISR2();
void IRAM_ATTR Timer0_ISR3();
void handleMessage(AdafruitIO_Data *data);

// variable globales

#define Button 34
#define T A7
#define R 4
#define A 17
#define V 16
#define pinServo 13
#define canalServo 3
#define FreqPWM 50
#define ResolutionPWM 12
#define GD 23
#define FD 22
#define AD 1
#define BD 3
#define DPD 32
#define DD 33
#define CD 25
#define ED 26
#define D1 19
#define D2 18
#define D3 5
#define IO_LOOP_DELAY 5000

uint8_t precaler = 80;
hw_timer_t * Timer0_Cfg = NULL;
float temperatura = 0;
float temp = 0.0;
boolean lectura = false;
uint8_t decenas = 0;
uint8_t unidades = 0;
uint8_t decimal; 

unsigned long lastUpdate = 0;

// set up the 'counter' feed
AdafruitIO_Feed *canalTemperatura = io.feed("Proyecto 1");

//Interrupciones
void IRAM_ATTR B1_ISR() {  
  lectura = true;
}

void IRAM_ATTR Timer0_ISR1() {
  configDispley(decenas, 1, 0);
}

void IRAM_ATTR Timer0_ISR2() {
  configDispley(unidades, 2, 1);
}

void IRAM_ATTR Timer0_ISR3() {
  configDispley(decimal, 3, 0);
}


//configuracion de perifericos
void setup() {
  analogReadResolution(12);
  configB1();
  configPWM();
  configTimer();
  decenas = 10;
  unidades = 10;
  decimal = 10;
  IRAM_ATTR B1_ISR();
  IRAM_ATTR Timer0_ISR1();
  delay(5);
  IRAM_ATTR Timer0_ISR2(); 
  delay(5);
  IRAM_ATTR Timer0_ISR3(); 
  Serial.begin(115200);
  pinMode(T, INPUT);
  pinMode(R, OUTPUT);
  pinMode(A, OUTPUT);
  pinMode(V, OUTPUT);
  digitalWrite(R,LOW);
  digitalWrite(A,LOW);
  digitalWrite(V,LOW); 
  pinMode(GD, OUTPUT);
  pinMode(FD, OUTPUT);
  pinMode(AD, OUTPUT);
  pinMode(BD, OUTPUT);
  pinMode(DPD, OUTPUT);
  pinMode(DD, OUTPUT);
  pinMode(CD, OUTPUT);
  pinMode(ED, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  digitalWrite(DPD, LOW);
  digitalWrite(GD, LOW);
  digitalWrite(FD, LOW);
  digitalWrite(AD, LOW);
  digitalWrite(BD, LOW);
  digitalWrite(DD, LOW);
  digitalWrite(CD, LOW);
  digitalWrite(ED, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  lectura = false;

  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  canalTemperatura->onMessage(handleMessage);

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  canalTemperatura->get();
}

//loop de aplicacion
void loop() {
  
  
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  if (millis() > (lastUpdate + IO_LOOP_DELAY)) {
    // save count to the 'counter' feed on Adafruit IO
    Serial.print("sending -> ");
    Serial.println(temp);
    canalTemperatura->save(temp);

    // after publishing, store the current time
    lastUpdate = millis();
  }



  if (lectura == true)
  {
    temperatura = analogRead(T);
    temp = ((temperatura * 330)/4095)+5;
    Serial.println(temp);
    obtenerCifras(temp);
    lectura = false;  
  }
  
  ledServo(temp);
  
  
  
  }
  
  



// put function definitions here:
// funcion para determinar el color de la led y el angulo del servo
void ledServo(float term){
  if (0 < term && term <= 23)
  {
   digitalWrite(R,LOW);
   digitalWrite(A,HIGH);
   digitalWrite(V,LOW);
   ledcWrite(canalServo, 210);
  }else if (23 < term && term <= 25)
  {
   digitalWrite(R,LOW);
   digitalWrite(A,LOW);
   digitalWrite(V,HIGH);
   ledcWrite(canalServo, 307);
  }else if (25 < term && term <= 27)
  {
   digitalWrite(R,HIGH);
   digitalWrite(A,LOW);
   digitalWrite(V,HIGH);
   ledcWrite(canalServo, 307);
  }else if (27 < term)
  {
   digitalWrite(R,HIGH);
   digitalWrite(A,LOW);
   digitalWrite(V,LOW);
   ledcWrite(canalServo, 409);
  }
}

//funcion de configuracion de la interrupcion del boton
void configB1(void){
  attachInterrupt(Button,&B1_ISR,HIGH);
}

//funcion de configuracion del canal para el servo
void configPWM(void){
  
  ledcSetup(canalServo, FreqPWM, ResolutionPWM);
  ledcAttachPin(pinServo, canalServo);
  ledcWrite(canalServo, 0);
}

//configuracion de lo que muestran los displays
void configDispley(uint8_t numero, uint8_t display, uint8_t punto){
  //seleccion de display
  switch (display)
  {
  case 1:
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    break;
  case 2:
    digitalWrite(D1, LOW);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, LOW);
    break;
  case 3:
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, HIGH);
    break;
  default:
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    break;
  }

  //activacion de punto
  if(punto){
    digitalWrite(DPD, HIGH);
  }else{
    digitalWrite(DPD, LOW);
  }   

  //seleccion de numero
  switch (numero)
  {
  case 0:
    digitalWrite(GD, LOW);
    digitalWrite(FD, HIGH);
    digitalWrite(AD, HIGH);
    digitalWrite(BD, HIGH);
    digitalWrite(DD, HIGH);
    digitalWrite(CD, HIGH);
    digitalWrite(ED, HIGH);
    break;
  case 1:
    digitalWrite(GD, LOW);
    digitalWrite(FD, LOW);
    digitalWrite(AD, LOW);
    digitalWrite(BD, HIGH);
    digitalWrite(DD, LOW);
    digitalWrite(CD, HIGH);
    digitalWrite(ED, LOW);
    break;
  case 2:
    digitalWrite(GD, HIGH);
    digitalWrite(FD, LOW);
    digitalWrite(AD, HIGH);
    digitalWrite(BD, HIGH);
    digitalWrite(DD, HIGH);
    digitalWrite(CD, LOW);
    digitalWrite(ED, HIGH);
    break;
  case 3:
     
    digitalWrite(GD, HIGH);
    digitalWrite(FD, LOW);
    digitalWrite(AD, HIGH);
    digitalWrite(BD, HIGH);
    digitalWrite(DD, HIGH);
    digitalWrite(CD, HIGH);
    digitalWrite(ED, LOW);
    break;
  case 4:
     
    digitalWrite(GD, HIGH);
    digitalWrite(FD, HIGH);
    digitalWrite(AD, LOW);
    digitalWrite(BD, HIGH);
    digitalWrite(DD, LOW);
    digitalWrite(CD, HIGH);
    digitalWrite(ED, LOW);
    break;
  case 5:
     
    digitalWrite(GD, HIGH);
    digitalWrite(FD, HIGH);
    digitalWrite(AD, HIGH);
    digitalWrite(BD, LOW);
    digitalWrite(DD, HIGH);
    digitalWrite(CD, HIGH);
    digitalWrite(ED, LOW);
    break;
  case 6:
     
    digitalWrite(GD, HIGH);
    digitalWrite(FD, HIGH);
    digitalWrite(AD, HIGH);
    digitalWrite(BD, LOW);
    digitalWrite(DD, HIGH);
    digitalWrite(CD, HIGH);
    digitalWrite(ED, HIGH);
    break;
  case 7:
     
    digitalWrite(GD, LOW);
    digitalWrite(FD, LOW);
    digitalWrite(AD, HIGH);
    digitalWrite(BD, HIGH);
    digitalWrite(DD, LOW);
    digitalWrite(CD, HIGH);
    digitalWrite(ED, LOW);
    break;
  case 8:
     
    digitalWrite(GD, HIGH);
    digitalWrite(FD, HIGH);
    digitalWrite(AD, HIGH);
    digitalWrite(BD, HIGH);
    digitalWrite(DD, HIGH);
    digitalWrite(CD, HIGH);
    digitalWrite(ED, HIGH);
    break;
  case 9:
     
    digitalWrite(GD, HIGH);
    digitalWrite(FD, HIGH);
    digitalWrite(AD, HIGH);
    digitalWrite(BD, HIGH);
    digitalWrite(DD, HIGH);
    digitalWrite(CD, HIGH);
    digitalWrite(ED, LOW);
    break;
  
  default:
    digitalWrite(GD, LOW);
    digitalWrite(FD, LOW);
    digitalWrite(AD, LOW);
    digitalWrite(BD, LOW);
    digitalWrite(DD, LOW);
    digitalWrite(CD, LOW);
    digitalWrite(ED, LOW);
    break;
  }
  
}

//funcion para obtener los valores de los displays
void obtenerCifras(float t){

    int ter = t * 10;
    decenas = ter/100;
    ter = ter - (decenas*100);
    unidades = ter/10;
    ter = ter - (unidades * 10);
    decimal = ter; 
    
  }

// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");
  Serial.println(data->value());
}

//configuracion de los timers para los displays
void configTimer(void) {

  Timer0_Cfg = timerBegin(0, precaler, true);

  timerAttachInterrupt(Timer0_Cfg, &Timer0_ISR1, true);
  timerAlarmWrite(Timer0_Cfg, 5000,true);
  timerAlarmEnable(Timer0_Cfg);

  Timer0_Cfg = timerBegin(1, precaler, true);

  timerAttachInterrupt(Timer0_Cfg, &Timer0_ISR2, true);
  timerAlarmWrite(Timer0_Cfg, 5000,true);
  timerAlarmEnable(Timer0_Cfg);  

  Timer0_Cfg = timerBegin(3, precaler, true);

  timerAttachInterrupt(Timer0_Cfg, &Timer0_ISR3, true);
  timerAlarmWrite(Timer0_Cfg, 5000,true);
  timerAlarmEnable(Timer0_Cfg); 

}
