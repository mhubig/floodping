#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "uart.h"
#include "rf12.h"
#include "i2cmaster.h"

#define AIRID "0102"

static union {
	unsigned char bytes[18];
	struct {
		unsigned short C1, C2, C3, C4, C5, C6, C7;
		unsigned char A, B, C, D;
	} Coeff;
} HP03_Cal;

static struct {
	unsigned short sens, off;
} HH10_Cal;

#define u8 unsigned char
#define u16 unsigned short

#define HP03_ADC_ADDR_W  0xEE
#define HP03_ADC_ADDR_R  0xEF

#define HP03_CAL_ADDR_W  0xA0
#define HP03_CAL_ADDR_R  0xA1

#define HH10_CAL_ADDR_W  0xA2
#define HH10_CAL_ADDR_R  0xA3

#define TIMER_ADDR_W	0xAA
#define TIMER_ADDR_R	0xAB

int RH[16];
int RHcount = 0;

void HP03_ReadOne(u8 what, u16 *v) {
	i2c_start_wait(HP03_ADC_ADDR_W);
	i2c_write(0xFF);
	i2c_write(what);
	i2c_stop();

	_delay_us(40000);

	i2c_start_wait(HP03_ADC_ADDR_W);
	i2c_write(0xFD);
	i2c_rep_start(HP03_ADC_ADDR_R);
	*v = i2c_readAck() << 8 | i2c_readNak();
	i2c_stop();
}

void HP03_Read(int *t, int *p) {

	u16 D1, D2;
	HP03_ReadOne(0xE8, &D2);
	HP03_ReadOne(0xF0, &D1);

	long Z = (D2 < HP03_Cal.Coeff.C5) ? HP03_Cal.Coeff.B : HP03_Cal.Coeff.A;
	long X = ((long) D2) - HP03_Cal.Coeff.C5;
	long dUT1 = (X * Z * X) >> 14;
	long dUT = (X - (dUT1 / (1 << HP03_Cal.Coeff.C)));

	*t = (250 + ((dUT * HP03_Cal.Coeff.C6) >> 16)) - (dUT >> HP03_Cal.Coeff.D);

	long Offs = (HP03_Cal.Coeff.C2 + (((HP03_Cal.Coeff.C4 - 1024) * dUT) >> 14))
			<< 2;
	long Sens = HP03_Cal.Coeff.C1 + ((HP03_Cal.Coeff.C3 * dUT) >> 10);
	X = ((Sens * (((long) D1) - 7168)) >> 14) - Offs;
	*p = ((X >> 5) + (((long) HP03_Cal.Coeff.C7) / 10));
}

ISR (TIMER1_CAPT_vect) {
	TCNT1 = 0;
}

ISR (TIMER1_COMPA_vect) {
	asm volatile ( "nop");
}

ISR (TIMER2_COMP_vect) {
	asm volatile ( "nop");
}

int t = 0;
int p = 0;

void readHP03() {
	HP03_Read(&t, &p);
	_delay_ms(1);
	HP03_Read(&t, &p);
}

void readHH10() {
	long freq = F_CPU / (ICR1);
	RHcount++;
	if (RHcount > 15) {
		RHcount = 0;
	}

	int RHvalue = ((HH10_Cal.off - freq) * HH10_Cal.sens) >> 12;
	RH[RHcount] = RHvalue;
}

void send() {

	long RHsumme = 0;
	for (int i = 0; i < 16; i++) {
		RHsumme += RH[i];
	}
	int RHvalue = RHsumme >> 4;
	int ttemp = t;
	int ptemp = p;

	char s[100];
	sprintf(s, "rh: %d, t:%d p:%d", RHvalue, ttemp, ptemp);
	uart_puts(s);

	char buf[32]; // = "g123456789012345678901234567890\0\0\0";
	buf[0] = 'f';
	memcpy(buf + 1, &RHvalue, 2);
	memcpy(buf + 3, &ttemp, 2);
	memcpy(buf + 5, &ptemp, 2);
	buf[7] = 0;
	memcpy(buf + 8, &RHvalue, 2);
	memcpy(buf + 10, &ttemp, 2);
	memcpy(buf + 12, &ptemp, 2);
	buf[14] = 0;

	rf12_init();
	rf12_setfreq(RF12FREQ868(868.3));
	rf12_setbandwidth(4, 1, 4);
	rf12_setbaud(666);
	rf12_setpower(0, 6);

	rf12_txdata(buf, 15);

	rf12_trans(0x8200);
	rf12_trans(0x0);

	_delay_ms(2);
}

#define TIMEOUT 1600000l

int main() {
	uart_init(UART_BAUD_SELECT(9600, F_CPU));
	uart_puts("Boot...\r\n\r\n");

	cli();
	wdt_reset();
	wdt_enable(WDTO_2S);

	TCCR1B = (1 << CS10);
	TCCR2 = (1 << COM20) | (1 << CS20) | (1 << WGM21);
	TIMSK = (1 << TICIE1) | (1 << OCIE2);
	TIFR = (1 << ICF1);
	OCR2 = 122;

	DDRB |= (1 << PB3); // OC2
	DDRB &= ~(1 << PB0); // ICP1
	PORTB &= ~(1 << PB0); // ICP1

	sei();

	uart_puts("i2c_init...");
	i2c_init();
	uart_puts("...done\r\n\r\n");
	wdt_reset();

	PORTD &= ~(1 << PD7); // XCLR low
	uart_puts("i2c wait HH10...");
	i2c_start_wait(HH10_CAL_ADDR_W);
	i2c_write(0x0A);
	i2c_rep_start(HH10_CAL_ADDR_R);
	HH10_Cal.sens = (i2c_readAck() << 8) | i2c_readAck();
	HH10_Cal.off = (i2c_readAck() << 8) | i2c_readNak();
	i2c_stop();
	uart_puts("done\r\n\r\n");
	PORTD |= (1 << PD7); // XCLR high
	wdt_reset();

	uart_puts("i2c wait HP03...");
	i2c_start_wait(HP03_CAL_ADDR_W);
	i2c_write(0x10);
	i2c_rep_start(HP03_CAL_ADDR_R);
	wdt_reset();

	for (int i = 0; i < 7; i++) {
		HP03_Cal.bytes[(i << 1) + 1] = i2c_readAck();
		HP03_Cal.bytes[(i << 1)] = i2c_readAck();
	}
	for (int i = 14; i < 17; i++) {
		HP03_Cal.bytes[i] = i2c_readAck();
	}
	HP03_Cal.bytes[17] = i2c_readNak();
	i2c_stop();
	uart_puts("done\r\n\r\n");
	wdt_reset();

	uart_puts("read HH10...");
	for (int uiTemp=0; uiTemp < 16; uiTemp++) {
		readHH10();
		_delay_ms(1);
		uart_puts(".");
	}
	uart_puts("done\r\n\r\n");
	wdt_reset();

	uart_puts("rf12_preinit...");
	rf12_preinit(AIRID);
	wdt_reset();
	uart_puts("...done\r\n\r\n");
	wdt_reset();

	rf12_init();
	rf12_setfreq(RF12FREQ868(868.3));
	rf12_setbandwidth(4, 1, 4);
	rf12_setbaud(666);
	rf12_setpower(0, 6);
	rf12_trans(0x8201);
	rf12_trans(0x0);

//	while(1)
//	{
//
//		uart_puts("rf12:i");
//		while(1)
//		{
//			uart_puts(".");
//			//sbi(RF_PORT_CS, CS);
//			uart_puts(".");
//		    _delay_us(100);
//			cbi(RF_PORT_CS, CS);
//	        //rf12_trans(0xC0E0); // AVR CLK: 10MHz
//	        _delay_us(100);
//		}
//
//		wdt_reset();
//
//		uart_puts("t");
//		rf12_txdata("xxx", 3);
//
//		wdt_reset();
//
//		uart_puts("u");
//		rf12_trans(0x8200);
//		rf12_trans(0x0);
//
//	}

	long uiC = TIMEOUT-1;
	while (1) {
		wdt_reset();

		uiC++;
		if ((uiC % (TIMEOUT) ) == 0) {
			uart_puts("preparing (");

			uiC = 0;

			uart_puts("HP03, ");
			readHP03();
			uart_puts("HH10) ");
			readHH10();

			uart_puts("S(");
			wdt_reset();
			send();
			uart_puts(")R");
			uart_puts("done\r\n\r\n");

//			i2c_start_wait(TIMER_ADDR_W);
//			i2c_write(0x99);
//			i2c_write(0x99);
//			i2c_write(0x99);
//			i2c_stop();
//
//			while(1)
//			{
//				set_sleep_mode(SLEEP_MODE_PWR_DOWN);
//				sleep_enable();
//				sleep_cpu();
//			}
		}

		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_enable();
		sleep_cpu();
	}
}
