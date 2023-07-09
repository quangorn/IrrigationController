#include <avr/io.h>
#include <avr/eeprom.h>
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

const uint32_t WAIT_CYCLES EEMEM = (uint32_t)24 * 3600;
const uint32_t RELAY_ON_SECONDS EEMEM = (uint32_t)10 * 60;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int main(void)
{
	//Init LED and Relay
	DDRB |= (1 << DDB0) | (1 << DDB3);

	while (1)
	{
		relayOff();
		ledOff();

		uint32_t count = eeprom_read_dword(&WAIT_CYCLES);
		while (count--) {
			_delay_ms(1000);
			ledToggle();
		}
		
		relayOn();
		ledOn();
		count = eeprom_read_dword(&RELAY_ON_SECONDS);
		while (count--) {
			_delay_ms(1000);
		}
	}
}
#pragma clang diagnostic pop
