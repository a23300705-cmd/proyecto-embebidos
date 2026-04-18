#include <Wire.h>
#include <SPI.h>
#include <TimerOne.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

const int btn  = 2;
const int led3 = 24;
const int CS   = 53;

int  humedad        = 0;
bool riegoAutoListo = false;

volatile bool botonPresionado = false;
volatile bool riegoActivo     = false;
volatile bool finRiego        = false;

unsigned long inicioRiego = 0;

// ── INT 1 — botón ───────────────────────────────────
void ISR_boton() {
    botonPresionado = true;
}

// ── INT 2 — fin de riego 15s ────────────────────────
ISR(TIMER4_COMPA_vect) {
    finRiego    = true;
    riegoActivo = false;
    TCCR4B      = 0; // detiene timer4
}

// ── INT 3 — TimerOne detecta humedad <= 33% ─────────
void isrTimerOne() {
    if (humedad <= 33 && !riegoActivo && !riegoAutoListo) {
        riegoAutoListo = true;
    }
}

void iniciarTimer4() {
    TCCR4A = 0; TCCR4B = 0; TCNT4 = 0;
    TCCR4B |= (1 << WGM42) | (1 << CS42) | (1 << CS40);
    OCR4A   = 234375; // 15s a 16MHz / 1024
    TIMSK4 |= (1 << OCIE4A);
}

void encenderBomba() {
    Wire.beginTransmission(8);
    Wire.write('1');
    Wire.endTransmission();
}

void apagarBomba() {
    Wire.beginTransmission(8);
    Wire.write('0');
    Wire.endTransmission();
}

void abrirValvula() {
    digitalWrite(CS, LOW);
    SPI.transfer('V'); SPI.transfer('1');
    digitalWrite(CS, HIGH);
}

void cerrarValvula() {
    digitalWrite(CS, LOW);
    SPI.transfer('V'); SPI.transfer('0');
    digitalWrite(CS, HIGH);
}

void parpadeoLED3() {
    digitalWrite(led3, !digitalRead(led3));
}

void iniciarRiego(int caso) {
    if (riegoActivo) return;
    riegoActivo = true;
    finRiego    = false;

    encenderBomba();
    abrirValvula();

    Timer1.initialize(5000000);
    Timer1.attachInterrupt(parpadeoLED3);

    iniciarTimer4();
}

void setup() {
    Wire.begin();
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV16);
    SPI.setDataMode(SPI_MODE0);

    lcd.begin(16, 2);

    pinMode(btn,  INPUT_PULLUP);
    pinMode(led3, OUTPUT);
    pinMode(CS,   OUTPUT);
    digitalWrite(CS, HIGH);

    attachInterrupt(digitalPinToInterrupt(btn), ISR_boton, FALLING);
    sei();
}

void loop() {
    // INT 1 — riego manual
    if (botonPresionado) {
        botonPresionado = false;

        if (humedad <= 66) {
            iniciarRiego(1);
            lcd.clear();
            lcd.setCursor(0, 0); lcd.print("Riego Manual");
        } else {
            lcd.clear();
            lcd.setCursor(0, 0); lcd.print("Hum>66% Denegado");
        }
    }

    // INT 3 — riego automático
    if (riegoAutoListo && !riegoActivo) {
        iniciarRiego(2);
        lcd.clear();
        lcd.setCursor(0, 0); lcd.print("Hum<=33% Auto");
    }
    if (humedad > 33) riegoAutoListo = false;

    // INT 2 — fin de riego
    if (finRiego) {
        finRiego = false;

        Timer1.detachInterrupt();
        digitalWrite(led3, LOW);

        apagarBomba();
        cerrarValvula();

        lcd.clear();
        lcd.setCursor(0, 0); lcd.print("Riego Finalizado");
    }
}