#include <avr/io.h>
#include <util/delay.h>

void ledToggle() {
    PORTB ^= 1 << PORTB0;
}

void ledOn() {
    PORTB |= 1 << PORTB0;
}

void ledOff() {
    PORTB &= ~(1 << PORTB0);
}

void relayOn() {
    PORTB &= ~(1 << PORTB3);
}

void relayOff() {
    PORTB |= 1 << PORTB3;
}

void delay1s() {
    _delay_ms(1000);
}

const uint16_t RELAY_ON_SECONDS = 15 * 60;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

int main(void) {
    //Init LED and Relay
    DDRB |= (1 << DDB0) | (1 << DDB3);

    relayOn();

    uint16_t delay = RELAY_ON_SECONDS;
    while (delay--) {
        delay1s();
        ledToggle();
    }

    relayOff();
    ledOff();

    while (1) {
        delay1s();
    }
}

#pragma clang diagnostic pop
