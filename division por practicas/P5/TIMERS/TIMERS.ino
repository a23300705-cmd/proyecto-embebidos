#include <TimerOne.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

const int led1 = 22;
const int led2 = 23;
const int led3 = 24;

int contadorTimer3 = 0;
volatile int humedad = 0;

// ── Timer3 ISR — LED1 toggle cada 5s ───────────────
ISR(TIMER3_COMPA_vect) {
    contadorTimer3++;
    if (contadorTimer3 >= 5) {
        contadorTimer3 = 0;
        digitalWrite(led1, !digitalRead(led1));
    }
}

// ── TimerOne ISR — LED3 parpadea cada 5s ───────────
void isrTimerOne() {
    digitalWrite(led3, !digitalRead(led3));
}

void setup() {
    lcd.begin(16, 2);

    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);

    // Timer3 — 1s, prescaler 1024
    cli();
    TCCR3A = 0; TCCR3B = 0; TCNT3 = 0;
    TCCR3B |= (1 << WGM32) | (1 << CS32) | (1 << CS30);
    OCR3A   = 15624;
    TIMSK3 |= (1 << OCIE3A);
    sei();

    // Timer5 — caudalímetro externo pin T5
    TCCR5A = 0; TCCR5B = 0; TCNT5 = 0;
    TCCR5B |= (1 << CS52) | (1 << CS51) | (1 << CS50);

    // TimerOne — 5s
    Timer1.initialize(5000000);
    Timer1.attachInterrupt(isrTimerOne);
}

void loop() {
    // LED2 — toggle cada 10 pulsos del caudalímetro
    if (TCNT5 >= 10) {
        TCNT5 = 0;
        digitalWrite(led2, !digitalRead(led2));
    }
}