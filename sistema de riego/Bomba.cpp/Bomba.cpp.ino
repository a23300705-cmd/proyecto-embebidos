#include "Bomba.h"

Bomba::Bomba(int direccionI2C) {
    _direccion = direccionI2C;
}

void Bomba::encender() {
    Wire.beginTransmission(_direccion);
    Wire.write('1');
    Wire.endTransmission();
}

void Bomba::apagar() {
    Wire.beginTransmission(_direccion);
    Wire.write('0');
    Wire.endTransmission();
}