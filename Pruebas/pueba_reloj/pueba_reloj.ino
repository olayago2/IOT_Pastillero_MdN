#include <Wire.h>
#include "RTClib.h"

// Usaremos el RTC DS3231
RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Iniciar el RTC
  if (!rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    while (1);
  }

  // Ajustar la hora si el RTC perdió energía
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  DateTime now = rtc.now();

  // Imprimir fecha y hora
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(3000); // Esperar 3 segundos antes de actualizar la hora
}
