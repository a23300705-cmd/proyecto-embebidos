#ifndef PANTALLA_H
#define PANTALLA_H

#include <Arduino.h>
#include <LiquidCrystal.h>

class Pantalla {
public:
    Pantalla(int rs, int en, int d4, int d5, int d6, int d7);
    void begin();
    void mostrarMensaje(const char* msg);
    void verificarMensaje();
    void actualizarContador();

    bool mostrando;

private:
    LiquidCrystal _lcd;
    unsigned long _inicioMensaje;
    unsigned long _tiempoAnterior;
    int           _contador;
};

#endif