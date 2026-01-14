#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#define F_CPU 16000000UL
//volatile uint8_t w=0,x=0,y=0,z=0;
//volatile uint8_t a=0,b=0,c=0,d=0;


 
int main (void)
{  
	DDRD    |= ((1 << PD2));
	DDRD    |= ((1 << PD3));
  	DDRD    |= ((1 << PD4));
	DDRD    |= ((1 << PD5));
	DDRD    &= ~(1 << PD6);
	DDRD    &= ~(1 << PD7);
	DDRB    &= ~(1 << PB0);
	DDRB    &= ~(1 << PB1);
	DDRB    |= ((1 << PB5));	
	while (1) {
		uint8_t w = (PIND >> PD6 ) & 1;
	        uint8_t x = (PIND >> PD7) & 1;
	        uint8_t y = (PINB >> PB0) & 1;
		uint8_t z = (PINB >> PB1) & 1;

		uint8_t A = (!w);
		uint8_t B = (w & (!x) & (!z)) | ((!w) & x);
		uint8_t C = ((!x) & y) | ((!w) & y) | (w & x & (!y));
		uint8_t D = ((!w) & z) | (w & x & y);

		PORTD |= (A << PD2);
		PORTD |= (B << PD3);
		PORTD |= (C << PD4);
		PORTD |= (D << PD5);

                _delay_ms(500);
	    	PORTB |= ((1 <<  PB5));
	    	_delay_ms(500);
		PORTB &= ~((1 <<  PB5));
 	 }
}
