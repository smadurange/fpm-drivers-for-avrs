/*
 * example_usage.c
 *
 */ 

#define bit_clr(a,b) ((a) &=~(1<<(b)))
#define bit_set(a,b) ((a) |= (1<<(b)))
#define bit_tst(a,b) ((a) & (1<<(b)))
#define bit_change(a,b) ((a) ^= (1<<(b)))

#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define Soft_UART_TX_PORT PORTD
#define Soft_UART_TX_DDR DDRD
#define Soft_UART_TX_PIN 3
#define Soft_UART_Baud 9600
#include "Soft_UART_Timer1.h"

#include "r503.h"

#define FPM_PIN       PD3
#define FPM_INTVEC    INT1_vect

static inline void uart_write(const char *s)
{
	for (; *s; s++) {
		Soft_UART_send_byte(*s);
	}
	Soft_UART_send_byte('\r');
	Soft_UART_send_byte('\n');
}

static inline void init_touch(void)
{
	DDRD &= ~(1 << FPM_PIN);
	EICRA = 0b00000000;
	EIMSK = (1 << INT1);
}

int main(void)
{
	struct fpm_cfg cfg;

	cli();
	Soft_UART_init();
	bit_set(DDRB,5);
	init_touch();

	sei();
	
	fpm_init();
	fpm_get_cfg(&cfg);

	if (fpm_get_count() == 0) {
		if (fpm_enroll()) {
			fpm_led_on(BLUE);
			_delay_ms(500);
			fpm_led_off();
			_delay_ms(500);
		} else {
			fpm_led_on(RED);
			_delay_ms(500);
			fpm_led_off();
			_delay_ms(500);
		}
	}

    while (1) 
    {	

		_delay_ms(2000);
    }

	return 0;
}

ISR(FPM_INTVEC)
{
	cli();
		if (fpm_match()) {
			fpm_led_on(BLUE);
			_delay_ms(1000);
			fpm_led_off();
			_delay_ms(500);
		} else {
			fpm_led_on(RED);
			_delay_ms(1000);
			fpm_led_off();
			_delay_ms(500);
		}
	sei();
}
