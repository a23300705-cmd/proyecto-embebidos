#ifndef VALVULA_H
#define VALVULA_H

#include <Arduino.h>
#include <SPI.h>

class Valvula {
public:
    Valvula(int pinCS);
    void begin();
    void abrir();
    void cerrar();

private:
    int _pinCS;
};

#endif