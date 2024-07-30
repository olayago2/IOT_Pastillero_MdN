#include <Adafruit_NeoPixel.h>

// Definir pines y otros parámetros
#define PIN_NEO_PIXEL 19
#define NUM_PIXELS 21
#define PIN_BUTTON 26
#define PIN_BUZZER 13

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

bool running = true;
unsigned long previousMillis = 0;
const long interval = 500; // intervalo de 500 ms

void setup() {
  strip.begin();
  strip.show(); // Inicializar todos los pixels a 'off'
  
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  
  unsigned long currentMillis = millis();

  // Verificar si el botón está presionado
  if (digitalRead(PIN_BUTTON) == LOW) {
    running = !running; // Cambiar el estado de running
    digitalWrite(PIN_BUZZER, HIGH); // Activar el buzzer
    delay(100); // Pequeño retardo para evitar rebotes
    digitalWrite(PIN_BUZZER, LOW); // Desactivar el buzzer
    delay(200); // Pequeño retardo adicional para evitar múltiples cambios de estado por un solo toque del botón
  }

  if (running && (currentMillis - previousMillis >= interval)) {
    previousMillis = currentMillis;
    shootingStarEffect();
  }
}

void shootingStarEffect() {
  // Posición inicial de la estrella fugaz
  int position = 0;

  // Color de la estrella fugaz
  // Encender un LED aleatorio con un color del arcoíris
  int randomPixel = random(NUM_PIXELS);
  uint32_t color = Wheel(random(256)); // Obtener un color aleatorio del arcoíris
  //uint32_t color = strip.Color(255, 255, 255); // Color blanco

  // Apagar todos los LEDs
  strip.clear();
  strip.show();

  // Encender el LED en la posición inicial
  strip.setPixelColor(position, color);
  strip.show();

  // Mover la estrella fugaz a través de la tira de LEDs
  for (int i = 0; i < NUM_PIXELS; i++) {
    delay(50); // Tiempo de retardo entre cada movimiento
    strip.clear(); // Apagar todos los LEDs

    // Mover la estrella fugaz a la siguiente posición
    position++;

    // Verificar si la estrella fugaz alcanzó el final de la tira de LEDs
    if (position >= NUM_PIXELS) {
      position = 0; // Reiniciar la posición al inicio
    }

    // Encender el LED en la nueva posición
    strip.setPixelColor(position, color);
    strip.show();
  }
}

void lightRandomPixel() {
  // Apagar todos los LEDs
  for (int i = 0; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }

  // Encender un LED aleatorio con un color del arcoíris
  int randomPixel = random(NUM_PIXELS);
  uint32_t color = Wheel(random(256)); // Obtener un color aleatorio del arcoíris
  strip.setPixelColor(randomPixel, color);
  strip.show();
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
