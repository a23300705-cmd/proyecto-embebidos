#include "Valvula.h"
#include <Arduino.h>

void Valvula::abrir() {
    Serial.println("V1");
}

void Valvula::cerrar() {
    Serial.println("V0");
}