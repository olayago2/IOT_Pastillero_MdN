#include <Adafruit_NeoPixel.h>

// Definiciones de pines y número de píxeles
#define PIN_NEO_PIXEL 19  // Pin de Arduino que se conecta al NeoPixel
#define NUM_PIXELS 21     // Número de LEDs (píxeles) en el NeoPixel
#define PIN_BUTTON 26     // Pin del ESP32 GPIO26 para el botón

// Crear el objeto NeoPixel
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

// Estado del botón
bool buttonState = false;
bool lastButtonState = false;

// Índice para los colores
int colorIndex = 0;

// Array de colores
uint32_t colors[] = {
  strip.Color(255, 0, 0),    // Rojo
  strip.Color(0, 255, 0),    // Verde
  strip.Color(0, 0, 255),    // Azul
  strip.Color(255, 255, 0),  // Amarillo
  strip.Color(0, 255, 255),  // Cian
  strip.Color(255, 0, 255),  // Magenta
  strip.Color(255, 165, 0),  // Naranja
  strip.Color(75, 0, 130),   // Índigo
  strip.Color(238, 130, 238),// Violeta
  strip.Color(255, 255, 255) // Blanco
};
const int numColors = sizeof(colors) / sizeof(colors[0]);

void setup() {
  // Inicializar el botón como entrada
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  
  // Inicializar el NeoPixel
  strip.begin();
  strip.show(); // Inicializar todos los píxeles a 'apagado'
  
  // Encender todos los LEDs con el primer color
  setAllPixelsColor(colors[colorIndex]);
}

void loop() {
  // Leer el estado del botón
  buttonState = digitalRead(PIN_BUTTON);

  // Detectar el cambio de estado del botón
  if (buttonState != lastButtonState) {
    // Esperar un momento para eliminar rebotes
    delay(50);
    
    // Actualizar el estado del botón
    lastButtonState = buttonState;

    // Si el botón está presionado
    if (buttonState == LOW) {
      // Incrementar el índice de color
      colorIndex = (colorIndex + 1) % numColors;
      
      // Establecer el color de todos los píxeles
      setAllPixelsColor(colors[colorIndex]);
    }
  }
}

// Función para encender todos los LEDs con un color específico
void setAllPixelsColor(uint32_t color) {
  for (int i = 0; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
