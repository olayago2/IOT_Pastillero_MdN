#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#define PIN_NEO_PIXEL 19  // Pin de Arduino que se conecta a NeoPixel
#define NUM_PIXELS 21      // Número de LEDs (píxeles) en NeoPixel
#define PIN_BUTTON 26     // Pin GPIO26 del ESP32

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);
int intervaloParpadeo = 500; // Intervalo de parpadeo en milisegundos
unsigned long previousMillis = 0;
bool isRainbowOn = true;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_BUTTON, INPUT_PULLUP); // Configurar el pin del botón
  NeoPixel.begin();                  // Inicializar objeto de la tira NeoPixel

  NeoPixel.clear();
  NeoPixel.show();
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= intervaloParpadeo) {
    previousMillis = currentMillis;

    if (isRainbowOn) {
      NeoPixel.clear();
    } else {
      NeoPixel.rainbow();
    }

    NeoPixel.show();
    isRainbowOn = !isRainbowOn; // Alternar el estado del arcoíris
  }
}

