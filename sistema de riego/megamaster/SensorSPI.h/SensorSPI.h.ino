#ifndef SENSORSPI_H
#define SENSORSPI_H

#include <Arduino.h>
#include <SPI.h>

class SensorSPI {
public:
    SensorSPI(int pinCS);
    void begin();
    void preguntar();   // envía 'H', LED1 HIGH
    int  recibir();     // lee byte de respuesta, LED1 LOW

private:
    int _pinCS;
};

#endif