#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

#include <Adafruit_NeoPixel.h>

// Definir pines y otros parámetros
#define PIN_NEO_PIXEL 19
#define NUM_PIXELS 21
#define PIN_BUTTON 26
#define PIN_BUZZER 13

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

unsigned long previousMillis = 0;
unsigned long buttonPressedMillis = 0;
const long interval = 600; // Intervalo de 250 ms (0.25 segundos)
const long longPressDuration = 1500; // Duración de pulsación larga en ms (1.5 segundos)

// Para rastrear LEDs encendidos en randomRainbow()
bool ledState[NUM_PIXELS] = {false};
int ledsOnCount = 0;

// change this to make the song slower or faster
int tempo = 200;

// change this to whichever pin you want to use
int buzzer = 13;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  // Super Mario Bros theme
  // Score available at https://musescore.com/user/2123/scores/2145
  // Theme by Koji Kondo
  
  NOTE_E5,8, NOTE_E5,8, REST,8, NOTE_E5,8, REST,8, NOTE_C5,8, NOTE_E5,8, //1
  NOTE_G5,4, REST,4, NOTE_G4,8, REST,4, 
  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // 3
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4,NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,
  
  REST,4, NOTE_G5,8, NOTE_FS5,8, NOTE_F5,8, NOTE_DS5,4, NOTE_E5,8,//7
  REST,8, NOTE_GS4,8, NOTE_A4,8, NOTE_C4,8, REST,8, NOTE_A4,8, NOTE_C5,8, NOTE_D5,8,
  REST,4, NOTE_DS5,4, REST,8, NOTE_D5,-4,
  NOTE_C5,2, REST,2,

  REST,4, NOTE_G5,8, NOTE_FS5,8, NOTE_F5,8, NOTE_DS5,4, NOTE_E5,8,//repeats from 7
  REST,8, NOTE_GS4,8, NOTE_A4,8, NOTE_C4,8, REST,8, NOTE_A4,8, NOTE_C5,8, NOTE_D5,8,
  REST,4, NOTE_DS5,4, REST,8, NOTE_D5,-4,
  NOTE_C5,2, REST,2,
};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;
int divider = 0, noteDuration = 0;
unsigned long previousNoteTime = 0;
int currentNote = 0;
bool isPlaying = true; // Variable para rastrear si la canción está en reproducción


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
      isPlaying = true; // Iniciar reproducción de la canción
      currentNote = 0; // Reiniciar el índice de la nota
      previousNoteTime = 0; // Reiniciar el tiempo de la nota anterior
    }
  } else {
    buttonPressedMillis = 0; // Reiniciar el temporizador de pulsación si el botón no está presionado
  }

  randomRainbow(); // Ejecutar randomRainbow()

  if (isPlaying) {
    canta();
  }
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

void canta() {
  unsigned long currentMillis = millis();
  if (currentNote < notes) {
    if (currentMillis - previousNoteTime >= noteDuration) {
      divider = melody[currentNote * 2 + 1];
      if (divider > 0) {
        noteDuration = (wholenote) / divider;
      } else {
        noteDuration = (wholenote) / abs(divider);
        noteDuration *= 1.5;
      }
      previousNoteTime = currentMillis;

      if (melody[currentNote * 2] == REST) {
        noTone(PIN_BUZZER);
      } else {
        tone(PIN_BUZZER, melody[currentNote * 2], noteDuration * 0.9);
      }
      currentNote++;
    }
  } else {
    noTone(PIN_BUZZER);
    isPlaying = false; // Detener la reproducción de la canción después de una vez
  }
}
