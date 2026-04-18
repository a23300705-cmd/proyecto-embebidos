#ifndef HUMEDAD_H
#define HUMEDAD_H

#include <Arduino.h>
#include <SPI.h>

class Humedad {
public:
    Humedad(int pinCS);
    void begin();
    void preguntar();   // primer transfer — envía 'H'
    int  recibir();     // segundo transfer — lee el valor

private:
    int _pinCS;
};

#endif