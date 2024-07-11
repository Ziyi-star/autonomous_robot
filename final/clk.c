#include "clk.h"

ISR(TIMER1_COMPA_vect) {
  cnt+=1;
  //0,01 sekunde
  if (cnt == 625) {
	cnt = 0;
	m_second+=1;
  }
  //10 Hz 
  if (cnt == 6250) {
	  // When the robot is not on the start field and the race has not started
	  if (!isCompleted && currentLap == 0) {
            USART_print("Hey you, you know what to do. :-)\n");
        }
	  
  }
  //1 Hz 
  if (cnt == 62500) {
	  // When the robot is on the start field and the race has not started
        if (isCompleted && currentLap == 0) {
            USART_print("Same story, different student ... boring, IES needs to refactor this course.\n");
        }
        // When the robot is racing
        if (isCompleted && currentLap > 0) {
            char round_msg[50];
            sprintf(round_msg, "Currently I go round #%d\n", currentLap);
            USART_print(round_msg);
        }
        if (isPaused){
			USART_print("Paused ... zzzZZZzzzZZZzzz ... Press P again to unpause me\n");
		}
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


