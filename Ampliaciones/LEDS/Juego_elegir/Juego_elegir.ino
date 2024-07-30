#include <Adafruit_NeoPixel.h>

// Definir pines y otros parámetros
#define PIN_NEO_PIXEL 19
#define NUM_PIXELS 21
#define PIN_BUTTON 26
#define PIN_BUZZER 13

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

unsigned long previousMillis = 0;
unsigned long buttonPressedMillis = 0;
const long interval = 250; // Intervalo de 250 ms (0.25 segundos)
const long longPressDuration = 1500; // Duración de pulsación larga en ms (1.5 segundos)

// Para rastrear LEDs encendidos en randomRainbow()
bool ledState[NUM_PIXELS] = {false};
int ledsOnCount = 0;

void setup() {
  strip.begin();
  strip.show(); // Inicializar todos los píxeles a 'off'

  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();

  // Verificar si el botón está presionado y si es una pulsación larga
  if (digitalRead(PIN_BUTTON) == LOW) {
    if (buttonPressedMillis == 0) {
      buttonPressedMillis = currentMillis;
    }
    if ((currentMillis - buttonPressedMillis >= longPressDuration)) {
      resetRandomRainbow(); // Reiniciar randomRainbow()
      buttonPressedMillis = 0; // Reiniciar el temporizador de pulsación
    }
  } else {
    buttonPressedMillis = 0; // Reiniciar el temporizador de pulsación si el botón no está presionado
  }

  randomRainbow(); // Ejecutar randomRainbow()
}

void resetRandomRainbow() {
  // Reiniciar variables para randomRainbow()
  previousMillis = millis();
  ledsOnCount = 0;
  for (int i = 0; i < NUM_PIXELS; i++) {
    ledState[i] = false;
  }
  strip.clear();
  strip.show();
}

void randomRainbow() {
  if (millis() - previousMillis >= interval) {
    if (ledsOnCount < NUM_PIXELS-1) {
      int randomPixel;
      do {
        randomPixel = random(NUM_PIXELS);
      } while (ledState[randomPixel]);

      uint32_t color = Wheel(random(256)); // Obtener un color aleatorio del arcoíris
      strip.setPixelColor(randomPixel, color);
      strip.show();

      ledState[randomPixel] = true;
      ledsOnCount++;
    }

    previousMillis = millis();
  }
}

// Función para generar colores del arcoíris
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
