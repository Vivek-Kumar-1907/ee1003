#include <avr/io.h>

void delay(int t){
	while(t--){
		while(((TIFR0>>2) & 1) == 0);
		TIFR0 |= (1<<2);
	}
}

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
	TCCR0B  = 0b00000101;
	while (1) {
		uint8_t w = (PIND >> PD6 ) & 1;
	        uint8_t x = (PIND >> PD7) & 1;
	        uint8_t y = (PINB >> PB0) & 1;
		uint8_t z = (PINB >> PB1) & 1;

		uint8_t A = (!w);
		uint8_t B = (w & (!x) & (!z)) | ((!w) & x);
		uint8_t C = ((!x) & y) | ((!w) & y) | (w & x & (!y));
		uint8_t D = ((!w) & z) | (w & x & y);
		PORTD &= ~((1<<PD2) | (1<<PD3) | (1<<PD4) | (1<<PD5));
		PORTD |= (A << PD2);
		PORTD |= (B << PD3);
		PORTD |= (C << PD4);
		PORTD |= (D << PD5);

                delay(61);
	    	PORTB |= ((1 <<  PB5));
	    	delay(61);
		PORTB &= ~((1 <<  PB5));
 	 }
}
