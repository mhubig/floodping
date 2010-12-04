#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define BIT(a) (1 << a)

#ifndef cbi
#define cbi(sfr, bit)     (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit)     (_SFR_BYTE(sfr) |= _BV(bit))
#endif

uint8_t getcap(uint8_t pin, uint8_t sampletime, uint8_t waittime) {
	DDRB &= ~pin;
	while (waittime--) {
		__asm__ ("nop");
	}
	while (!(PINB & pin) && --sampletime)
		;
	DDRB |= pin;
	return sampletime;
}

int main() {
	while (1) {
		int i = 0;
		int n = 11;
		while (--n) {
			i += getcap(1 << PB3, 50, 40);
		}
		i /= 10;
		if (i < 42) {
			PORTB |= (1 << PB0);
		} else if (i > 45) {
			PORTB &= ~(1 << PB0);
		}
		_delay_ms(100);
	}
}
