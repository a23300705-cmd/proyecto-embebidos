#include "Sensor.h"

Sensor:Sensor(int pin) {
    _pin = pin;
}

void Sensor::begin() {}

int Sensor::leerHumedad() {
    int valor = analogRead(_pin);
    return map(valor, 0, 1023, 100, 0);
}