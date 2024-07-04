#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "iesusart.h"

#define OR_HEARTBEAT PORTB
#define OP_HEARTBEAT PB5
#define DR_HEARTBEAT DDRB
#define DP_HEARTBEAT DDB5

// We want to toggle the LED every 0.5s.
// In every second, 16E6/256=62500 interrupts are fired.
// In every half-second 62500/2=31250 ticks are fired.
uint16_t cnt = 0;
ISR(TIMER1_COMPA_vect) {
  cnt+=1;
  //halbe sekunde
  if (cnt == 31250) {
	OR_HEARTBEAT ^= (1 << OP_HEARTBEAT); // Toggle-trick.
	cnt = 0;
  }
}

void setup_heartbeat_timer() {
  cli();                   // Disable interrupts!
  TCCR1B |= (1 << CS10);   // Prescaler: 1 => 16E6 ticks/second
  TCCR1B |= (1 << WGM12);  // Use Timer 1 in CTC-mode
  TIMSK1 |= (1 << OCIE1A); // Enable compare-match-interrupt for OCR1A
  OCR1A = 255;             // Every 16E6/256 ticks COMPA_vect is fired.
                           // This equals an (non-existent) 512-clock-
                           // divisor. We need this information for
                           // later calculations.
                           // BTW: Keep in mind that there is one more
                           // OCR-register for timer 1, which you can
                           // use to do some more neat stuff.
                           // BTW^2: Remember that OCR1[A|B] are 16 bit!
  sei();                   // Enable interrupts!
}

int main(void) {
  DR_HEARTBEAT |= (1 << DP_HEARTBEAT);
	
  USART_init(UBRR_SETTING);

  //setup_heartbeat_timer(); 
  
  unsigned char some_letter = 'A' - 1;

  while(1) {
    OR_HEARTBEAT &= ~(1 << OP_HEARTBEAT);
    _delay_ms(500);
    OR_HEARTBEAT |= (1 << OP_HEARTBEAT);
    _delay_ms(500);
    
    some_letter++;
    USART_transmitByte(some_letter);
    USART_transmitByte('\n');
    if (some_letter == 'Z') {
		some_letter = 'A' - 1;
	}
  }

  return 0;

}
