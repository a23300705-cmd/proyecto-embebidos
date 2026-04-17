#include "Relee.h"

Rele::Rele(int pin) {
    _pin = pin;
}

void Rele::begin() {
    pinMode(_pin, OUTPUT);
}

void Rele::encender() {
    digitalWrite(_pin, HIGH);
}

void Rele::apagar() {
    digitalWrite(_pin, LOW);
}