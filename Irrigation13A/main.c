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

//https://www.electro-tech-online.com/tools/thermistor-resistance-calculator.php
//8grad  1.55V = 79
//10grad 1.66V = 84
//18grad 2.11V = 107
//20grad 2.22V = 113
//25grad 2.50V = 127
//30grad 2.77V = 141
//35grad 3.03V = 154
//40grad 3.27V = 166
const uint8_t TEMP_MIN EEMEM = 84;
const uint8_t TEMP_AVG EEMEM = 107;
const uint8_t TEMP_MAX EEMEM = 141;

const uint32_t WAIT_CYCLES EEMEM = (uint32_t)12 * 3600;
const uint32_t RELAY_ON_SECONDS EEMEM = (uint32_t)4 * 60;

void delay10ms(uint16_t period) {
	while (period--) {
		_delay_ms(10);
	}
}

int main(void)
{
	//Init ADC
	ADCSRA = (1 << ADEN)	//Enable ADC
			| (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  //Set prescaler = 128 (16MHz/128 = 125kHz)
	ADMUX |= (1 << ADLAR)	//Left adjust ADC result to allow easy 8 bit reading
			| (1 << MUX1);	//Select ADC2 (PB4)
			
	//Init LED and Relay
	DDRB |= (1 << DDB0) | (1 << DDB3);
	
	while (1)
	{
		relayOff();
		ledOff();
		
		uint32_t count = eeprom_read_dword(&WAIT_CYCLES);
		while (count--) {
			ADCSRA |= (1 << ADSC);  //Start A2D Conversions			
			uint8_t tempMin = eeprom_read_byte(&TEMP_MIN);
			uint8_t tempMax = eeprom_read_byte(&TEMP_MAX);
			int16_t tempAvg = eeprom_read_byte(&TEMP_AVG);
			int16_t adcVal = ADCH;
			if (adcVal < tempMin) {
				adcVal = tempMin;
			} else if (adcVal > tempMax) {
				adcVal = tempMax;
			}
			adcVal -= tempAvg;
			if (adcVal >= 0) {
				delay10ms(100 - (adcVal * 50 / (tempMax - tempAvg)));	//0.5s min
			} else {
				delay10ms(100 - (adcVal * 100 / (tempAvg - tempMin)));	//2s max
			}
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



