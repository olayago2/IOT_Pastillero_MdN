#include <Adafruit_NeoPixel.h>

// Definir pines y otros parámetros
#define PIN_NEO_PIXEL 19
#define NUM_PIXELS 21
#define PIN_BUTTON 26
#define PIN_BUZZER 13
// PIN SDA 21
// PIN SCL 22

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

// Modos 
#define MODE_RANDOM_RAINBOW 1
#define MODE_ALL_PURPLE 2
#define MODE_RAINBOW_BLINK 3
#define MODE_ALL_CIAN 4
#define MODE_RAINBOW_GRADIENT 5

bool running = true;
unsigned long previousMillis = 0;
unsigned long buttonPressedMillis = 0;
unsigned long ledChangeMillis = 0;
const long interval = 250; // intervalo de 250 ms (0.25 segundos)
const long longPressDuration = 1000; // duración de pulsación larga en ms (3 segundos)
int gradientSpeed = 20; // Velocidad de desplazamiento del gradiente
int gradientPosition = 0; // Posición inicial del gradiente

int currentMode = MODE_RANDOM_RAINBOW;
int currentLED = 0;

// Orden de LEDs
int ordenLED[21] = {0, 13, 14, 1, 12, 15, 2, 11, 16, 3, 10, 17, 4, 9, 18, 5, 8, 19, 6, 7, 20};

// Para rastrear LEDs encendidos en randomRainbow()
bool ledState[NUM_PIXELS] = {false};
int ledsOnCount = 0;

int offset = 0; // Desplazamiento inicial del gradiente

void setup() {
  strip.begin();
  strip.show(); // Inicializar todos los pixels a 'off'
  
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
      changeMode(); // Cambiar al siguiente modo
      buttonPressedMillis = 0; // Reiniciar el temporizador de pulsación
    }
  } else {
    buttonPressedMillis = 0; // Reiniciar el temporizador de pulsación si el botón no está presionado
  }

  // Ejecutar la función correspondiente al modo actual
  if (running) {
    switch (currentMode) {

      case MODE_RANDOM_RAINBOW:
        randomRainbow();
        break;
      case MODE_ALL_PURPLE:
        allPurple(currentMillis);
        break;
      case MODE_RAINBOW_BLINK:
        rainbowBlink(currentMillis);
        break;
      case MODE_ALL_CIAN:
        allCian(currentMillis);
        break;
      case MODE_RAINBOW_GRADIENT:
        rainbowGradient(); 
        break;  
       
    }
  }
}

void changeMode() {
  // Cambiar al siguiente modo
  currentMode++;
  if (currentMode > MODE_RAINBOW_GRADIENT) {
    currentMode = MODE_RANDOM_RAINBOW;
  }

  // Reiniciar variables según el nuevo modo
  previousMillis = millis();
  ledChangeMillis = millis();
  ledsOnCount = 0;
  currentLED = 0;
  for (int i = 0; i < NUM_PIXELS; i++) {
    ledState[i] = false;
  }
  strip.clear();
  strip.show();
}

void randomRainbow() {
  if (millis() - previousMillis >= interval) {
    if (ledsOnCount < NUM_PIXELS) {
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
    else {
      // Reiniciar todos los LEDs y comenzar de nuevo
      for (int i = 0; i < NUM_PIXELS; i++) {
        ledState[i] = false;
      }
      ledsOnCount = 0;
      strip.clear();
      strip.show();
    }

    previousMillis = millis();
  }
}

void allPurple(unsigned long currentMillis) {
  // Inicializar todos los píxeles a blanco al inicio
  if (currentLED == 0) {
    strip.fill(strip.Color(255, 255, 255));
    strip.show();
  }

  // Encender los LEDs en el orden especificado con un intervalo de 250 ms
  if (currentMillis - ledChangeMillis >= interval) {
    if (currentLED < NUM_PIXELS) {
      strip.setPixelColor(ordenLED[currentLED], strip.Color(128, 0, 128)); // Púrpura
      strip.show();
      currentLED++;
    } else {
      // Reiniciar para empezar de nuevo
      currentLED = 0;
    }
    ledChangeMillis = currentMillis;
  }
}

void rainbowBlink(unsigned long currentMillis) {
  static bool ledsOn = false;
  
  if (currentMillis - ledChangeMillis >= interval) {
    if (ledsOn) {
      strip.clear(); // Apagar todos los LEDs
    } else {
      for (int i = 0; i < NUM_PIXELS; i++) {
        strip.setPixelColor(i, Wheel((i * 256 / NUM_PIXELS) & 255)); // Rueda de colores del arcoíris
      }
    }
    strip.show();
    ledsOn = !ledsOn;
    ledChangeMillis = currentMillis;
  }
}

void allCian(unsigned long currentMillis) {
  // Inicializar todos los píxeles a blanco al inicio
  if (currentLED == 0) {
    strip.fill(strip.Color(255, 255, 255));
    strip.show();
  }

  // Encender los LEDs con un intervalo de 250 ms
  if (currentMillis - ledChangeMillis >= interval) {
    if (currentLED < NUM_PIXELS) {
      strip.setPixelColor(currentLED, strip.Color(0, 128, 128)); // Cian
      strip.show();
      currentLED++;
    } else {
      // Reiniciar para empezar de nuevo
      currentLED = 0;
    }
    ledChangeMillis = currentMillis;
  }
}

void rainbowGradient() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Desplazar el gradiente
    gradientPosition += gradientSpeed;
    if (gradientPosition >= 256) {
      gradientPosition = 0;
    }

    // Aplicar el gradiente de arcoíris a los LEDs
    for (int i = 0; i < NUM_PIXELS; i++) {
      int colorIndex = (i * 256 / NUM_PIXELS + gradientPosition) & 255;
      strip.setPixelColor(ordenLED[i], Wheel(colorIndex));
    }

    strip.show(); // Mostrar los cambios en los LEDs
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
