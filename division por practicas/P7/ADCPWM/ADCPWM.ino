//Este bloque es del MASTER (Mega)
#include <SPI.h>

const int CS = 53;
int humedad  = 0;

void setup() {
    Serial.begin(9600);
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV16);
    SPI.setDataMode(SPI_MODE0);

    pinMode(CS, OUTPUT);
    digitalWrite(CS, HIGH);
}

void pedirHumedad() {
    digitalWrite(CS, LOW);
    SPI.transfer('H');
    digitalWrite(CS, HIGH);
}

int recibirHumedad() {
    delay(10);
    digitalWrite(CS, LOW);
    int val = SPI.transfer(0x00);
    digitalWrite(CS, HIGH);
    return val;
}

void loop() {
    pedirHumedad();
    humedad = recibirHumedad();

    Serial.print("Humedad: ");
    Serial.print(humedad);
    Serial.println("%");

    delay(1000);
}


//Este bloque es del ESCLAVO (Mega esclavo)
#include <TimerOne.h>

const int pot    = A0;
const int ledPWM = 11;

volatile int humedad  = 0;
volatile int valorADC = 0;
volatile char comando = 0;

ISR(SPI_STC_vect) {
    char recibido = SPDR;
    if (recibido == 'H') {
        SPDR    = (uint8_t)humedad;
        comando = 0;
    }
}

void setup() {
    pinMode(MISO, OUTPUT);
    pinMode(ledPWM, OUTPUT);

    // SPI modo esclavo
    SPCR |= _BV(SPE);
    SPCR &= ~_BV(MSTR);
    SPCR |= _BV(SPIE);
    sei();

    // TimerOne PWM 10 bits
    Timer1.initialize(1000);
    Timer1.pwm(ledPWM, 0);
}

void loop() {
    valorADC = analogRead(pot);
    humedad  = map(valorADC, 0, 1023, 100, 0);

    // brillo inversamente proporcional a humedad
    Timer1.setPwmDuty(ledPWM, 1023 - valorADC);
}