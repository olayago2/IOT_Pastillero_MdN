#include <Adafruit_NeoPixel.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <Wire.h>
#include <RTClib.h>

#define PIN_NEO_PIXEL 19  // Pin de Arduino que se conecta a NeoPixel
#define NUM_PIXELS 21     // Número de LEDs (píxeles) en NeoPixel
#define PIN_BUTTON 26     // Pin GPIO26 del ESP32

RTC_DS3231 rtc;

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

int ordenLED[21] = { 0, 13, 14, 1, 12, 15, 2, 11, 16, 3, 10, 17, 4, 9, 18, 5, 8, 19, 6, 7, 20 };
int indexPastilla = 0;
int pastillaTomada = 1;

void setup() {
  Serial.begin(115200);
  //pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);  // Configurar el pin del botón
  NeoPixel.begin();                   // Inicializar objeto de la tira NeoPixel
  
  if (!rtc.begin()) {
    Serial.println("No se pudo iniciar el RTC");
    while (1); // Esperar indefinidamente si no se puede inicializar el RTC
  }

  DateTime now = rtc.now();
  setTime(now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year() - 2000);

  

  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), boton, FALLING);  // Asignar la función 
}

void loop() {
  Alarm.delay(10);  // Esperar que las alarmas se activen
  //NeoPixel.rainbow();
  NeoPixel.show();
  
  takePill();
  delay(60);
}


void takePill(void) {
  Serial.println("Ejecuta la funcion");
  //alarmaActivada = true;
  pastillaTomada = 0;

    //NeoPixel.setPixelColor(0, NeoPixel.Color(0, 200, 0));
    NeoPixel.setPixelColor(ordenLED[indexPastilla], NeoPixel.Color(100, 0, 100));
    NeoPixel.show();
    //Alarm.timerOnce(30, noTomo);  // called once after 30 seconds

    indexPastilla++;
}

void boton(void) {
  delay(20);
  
    //NeoPixel.setPixelColor(1, NeoPixel.Color(0, 0, 0));  // Apagar el LED
    NeoPixel.clear();
    //NeoPixel.setPixelColor(ordenLED[indexPastilla], NeoPixel.Color(0, 0, 0));  // Apagar el LED
    NeoPixel.show();
    pastillaTomada = 1;
}



