#include "Valvula.h"

Valvula::Valvula(int pinCS) {
    _pinCS = pinCS;
}

void Valvula::begin() {
    pinMode(_pinCS, OUTPUT);
    digitalWrite(_pinCS, HIGH);
}

void Valvula::abrir() {
    digitalWrite(_pinCS, LOW);
    SPI.transfer('V');
    SPI.transfer('1');
    digitalWrite(_pinCS, HIGH);
}

void Valvula::cerrar() {
    digitalWrite(_pinCS, LOW);
    SPI.transfer('V');
    SPI.transfer('0');
    digitalWrite(_pinCS, HIGH);
}