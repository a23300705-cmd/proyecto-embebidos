#include "SensorSPI.h"

SensorSPI::SensorSPI(int pinCS) {
    _pinCS = pinCS;
}

void SensorSPI::begin() {
    pinMode(_pinCS, OUTPUT);
    digitalWrite(_pinCS, HIGH);
}

void SensorSPI::preguntar() {
    digitalWrite(_pinCS, LOW);
    SPI.transfer('H');
    digitalWrite(_pinCS, HIGH);
}

int SensorSPI::recibir() {
    delay(10); // espera que esclavo prepare dato
    digitalWrite(_pinCS, LOW);
    int val = SPI.transfer(0x00);
    digitalWrite(_pinCS, HIGH);
    return val;
}