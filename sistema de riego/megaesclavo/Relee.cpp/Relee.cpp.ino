#include "Relee.h"

Relee::Relee(int pin) {
    _pin = pin;
}

void Relee::begin() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
}

void Relee::abrir() {
    digitalWrite(_pin, HIGH);
}

void Relee::cerrar() {
    digitalWrite(_pin, LOW);
}