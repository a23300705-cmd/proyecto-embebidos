#include "Pantalla.h"

Pantalla::Pantalla(int rs, int en, int d4, int d5, int d6, int d7)
    : _lcd(rs, en, d4, d5, d6, d7)
{
    mostrando       = false;
    _inicioMensaje  = 0;
    _tiempoAnterior = 0;
    _contador       = 0;
}

void Pantalla::begin() {
    _lcd.begin(16, 2);
}

void Pantalla::mostrarMensaje(const char* msg) {
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print(msg);
    mostrando      = true;
    _inicioMensaje = millis();
}

void Pantalla::verificarMensaje() {
    if (mostrando && millis() - _inicioMensaje >= 5000) {
        mostrando = false;
        _lcd.clear();
    }
}

void Pantalla::actualizarContador() {
    if (!mostrando && millis() - _tiempoAnterior >= 500) {
        _tiempoAnterior = millis();
        if (++_contador > 99) _contador = 0;

        _lcd.setCursor(0, 0);
        _lcd.print("Cont: ");
        _lcd.print(_contador);
    }
}