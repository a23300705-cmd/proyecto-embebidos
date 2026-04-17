#include <Wire.h>
#include <TimerOne.h>
#include "Bomba.h"
#include "Valvula.h"
#include "Humedad.h"
#include "Riego.h"
#include "Pantalla.h"

// pines
int led1 = 22;
int led2 = 23;
int led3 = 24;
int btn  = 2;
int T5   = 47;

// objetos
Bomba    bomba(8);
Valvula  valvula;
Humedad  sensor;
Riego    riego(bomba, valvula, led3);
Pantalla pantalla(12, 11, 10, 9, 8, 7);

// estado
int humedad       = 0;
bool riegoAutoListo = false;

// flags de ISR
volatile bool botonPresionado = false;

// ── callbacks globales ──────────────────────────────
void parpadeoLED3() {
    digitalWrite(led3, !digitalRead(led3));
}

void ISR_boton() {
    botonPresionado = true;
}

ISR(TIMER3_COMPA_vect) {
    static bool estado = false;
    estado = !estado;

    if (estado) {
        digitalWrite(led1, HIGH);
        sensor.preguntar();
    } else {
        digitalWrite(led1, LOW);
        int val = sensor.recibir();
        if (val >= 0) humedad = val;
    }
}
// ───────────────────────────────────────────────────

void setup() {
    Serial.begin(9600);
    Wire.begin();

    pantalla.begin();

    pinMode(btn,  INPUT_PULLUP);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(T5,   INPUT);

    attachInterrupt(digitalPinToInterrupt(btn), ISR_boton, FALLING);

    // Timer3 — comunicación (1 segundo)
    cli();
    TCCR3A = 0;
    TCCR3B = 0;
    TCNT3  = 0;
    TCCR3B |= (1 << WGM32) | (1 << CS32) | (1 << CS30);
    OCR3A   = 78124;
    TIMSK3 |= (1 << OCIE3A);
    sei();

    // Timer5 — caudalímetro
    TCCR5A = 0;
    TCCR5B = 0;
    TCNT5  = 0;
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
            pantalla.mostrarMensaje("Riego Denegado");
        }
    }

    // riego automático
    if (humedad <= 33 && !riego.activo && !riegoAutoListo) {
        riegoAutoListo = true;
        riego.iniciar(2);
        pantalla.mostrarMensaje("Riego Auto");
    }
    if (humedad > 33) riegoAutoListo = false;

    // duración 15s
    riego.verificarDuracion();

    // fin de riego
    if (riego.fin) {
        riego.finalizar();
        pantalla.mostrarMensaje("Riego Finalizado");
    }
}