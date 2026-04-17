#include "Sensor.h"

Sensor::Sensor(int pinPot) {
    _pin = pinPot;
}

void Sensor::begin() {
    // analogRead no necesita pinMode
}

int Sensor::leerValorRaw() {
    return analogRead(_pin);
}

int Sensor::leerHumedad() {
    return map(analogRead(_pin), 0, 1023, 100, 0);
}