#include "Riego.h"
#include <Arduino.h>

Riego::Riego(Bomba& bomba, Valvula& valvula, int pinLED3)
    : _bomba(bomba), _valvula(valvula), _pinLED3(pinLED3)
{
    activo = false;
    fin = false;
    tipo = 0;
    _inicioRiego = 0;
}

void Riego::iniciar(int caso) {
    if (activo) return;

    activo = true;
    fin = false;
    tipo = caso;
    _inicioRiego = millis();

    _bomba.encender();
    _valvula.abrir();

    TimerOne.initialize(5000000);
    TimerOne.attachInterrupt(parpadeoLED3);
}

void Riego::verificarDuracion() {
    if (activo && millis() - _inicioRiego >= 15000) {
        activo = false;
        fin = true;

        TimerOne.detachInterrupt();
        digitalWrite(_pinLED3, LOW);
    }
}

void Riego::finalizar() {
    fin = false;
    _bomba.apagar();
    _valvula.cerrar();
}