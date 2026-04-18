#include <Wire.h>
#include <SPI.h>
#include <TimerOne.h>
#include <LiquidCrystal.h>
#include "Bomba.h"
#include "Valvula.h"
#include "SensorSPI.h"
#include "Riego.h"
#include "Pantalla.h"

// pines
const int led1 = 22;
const int led2 = 23;
const int led3 = 24;
const int btn  = 2;
const int CS   = 53;

// objetos
Bomba     bomba(8);
Valvula   valvula(CS);
SensorSPI sensor(CS);
Riego     riego(bomba, valvula, led3);
Pantalla  pantalla(12, 11, 10, 9, 8, 7);

int  humedad        = 0;
bool riegoAutoListo = false;

volatile bool botonPresionado    = false;
volatile bool estadoComunicacion = false;

// ── callbacks globales ──────────────────────────────
void parpadeoLED3() {
    digitalWrite(led3, !digitalRead(led3));
}

void ISR_boton() {
    botonPresionado = true;
}

ISR(TIMER3_COMPA_vect) {
    estadoComunicacion = !estadoComunicacion;

    if (estadoComunicacion) {
        digitalWrite(led1, HIGH);
        sensor.preguntar();
    } else {
        digitalWrite(led1, LOW);
        int val = sensor.recibir();
        if (val >= 0) humedad = val;
    }
}

void setup() {
    Wire.begin();
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV16);
    SPI.setDataMode(SPI_MODE0);

    pantalla.begin();
    bomba.begin();
    valvula.begin();
    sensor.begin();

    pinMode(btn,  INPUT_PULLUP);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(btn), ISR_boton, FALLING);

    // Timer3 — 5 s
    cli();
    TCCR3A = 0; TCCR3B = 0; TCNT3 = 0;
    TCCR3B |= (1 << WGM32) | (1 << CS32) | (1 << CS30);
    OCR3A   = 78124;
    TIMSK3 |= (1 << OCIE3A);
    sei();

    // Timer5 — caudalímetro externo pin T5
    TCCR5A = 0; TCCR5B = 0; TCNT5 = 0;
    TCCR5B |= (1 << CS52) | (1 << CS51) | (1 << CS50);
}

void loop() {
    // caudalímetro
    if (TCNT5 >= 10) {
        TCNT5 = 0;
        digitalWrite(led2, !digitalRead(led2));
    }

    pantalla.verificarMensaje();
    pantalla.actualizarContador();

    // botón — riego manual
    if (botonPresionado) {
        botonPresionado = false;

        if (humedad <= 66) {
            riego.iniciar(1);
            pantalla.mostrarMensaje("Riego Manual");
        } else {
            pantalla.mostrarMensaje("Hum>66% Denegado");
        }
    }

    // riego automático
    if (humedad <= 33 && !riego.activo && !riegoAutoListo) {
        riegoAutoListo = true;
        riego.iniciar(2);
        pantalla.mostrarMensaje("Hum<=33% RiegoAuto");
    }
    if (humedad > 33) riegoAutoListo = false;

    // duración 15 s
    riego.verificarDuracion();

    // fin de riego
    if (riego.fin) {
        riego.finalizar();
        pantalla.mostrarMensaje("Riego Finalizado");
    }
}