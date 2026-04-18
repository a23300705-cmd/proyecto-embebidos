#include "Humedad.h"

Humedad::Humedad(int pinCS) {
    _pinCS = pinCS;
}

void Humedad::begin() {
    pinMode(_pinCS, OUTPUT);
    digitalWrite(_pinCS, HIGH);
}

void Humedad::preguntar() {
    digitalWrite(_pinCS, LOW);
    SPI.transfer('H');
    digitalWrite(_pinCS, HIGH);
}

int Humedad::recibir() {
    delay(10); // espera que el esclavo prepare el dato
    digitalWrite(_pinCS, LOW);
    int val = SPI.transfer(0x00);
    digitalWrite(_pinCS, HIGH);
    return val;
}