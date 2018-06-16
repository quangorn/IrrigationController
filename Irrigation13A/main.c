/*
 * Irrigation13A.c
 *
 * Created: 20.05.2018 16:57:04
 * Author : Vladimir
 */ 

#define F_CPU 9600000

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

const uint32_t WAIT_CYCLES EEMEM = (uint32_t)12 * 3600;
const uint32_t RELAY_ON_SECONDS EEMEM = (uint32_t)4 * 60;

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



