#include "test_line_rename.h"

uint16_t cnt = 0;
uint16_t m_second = 0;

ISR(TIMER1_COMPA_vect) {
  cnt+=1;
  if (cnt == 625) {
	cnt = 0;
	m_second+=1;
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


// Leichte Abbiegen nach rechts
void turn_right(){
    // Set the duty cycles for PD5/PD6
    //links
    setDutyCycle(PD5, 255);
    //rechts
    setDutyCycle(PD6, 180);
    // Set IN1 to HIGH and don't set IN2 to HIGH (leave LOW) -> Left motors FORWARD
    PORTD |= (1 << PIN_IN1_FORWARD_LEFT); 
    // Set IN2 to LOW, nothing else HIGH -> No motors turn
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    // Set IN3 to HIGH and don't set anything else to HIGH -> Right motors BACKWARD
    PORTB |= (1 << PIN_IN3_BACKWARD_RIGHT);
    // Set IN4 to LOW, nothing else HIGH -> No motors turn
    PORTB &= ~(1 << PIN_IN4_FORWARD_RIGHT);

}

// grosse Abbiegen nach links
void turn_left(){
    // Set the duty cycles for PD5/PD6
    //links
    setDutyCycle(PD5, 180);
    //rechts
    setDutyCycle(PD6, 255);
    // links  FORWARD, 0
    PORTD &= ~(1 << PIN_IN1_FORWARD_LEFT);
    // links BACKWARD 1
    PORTB |= (1 << PIN_IN2_BACKWARD_LEFT);
    // RIGHT FORWARD 1
    PORTB |= (1 << PIN_IN4_FORWARD_RIGHT);
    // Right BACKWARD 0
    PORTB &= ~(1 << PIN_IN3_BACKWARD_RIGHT);
}

// Leichte Abbiegen nach links
void drive_left(){
    // Set the duty cycles for PD5/PD6
    //links
    setDutyCycle(PD5, 155);
    //rechts
    setDutyCycle(PD6, 255);
    // links  FORWARD, 1
    PORTD |= (1 << PIN_IN1_FORWARD_LEFT);
    // links BACKWARD 1
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    // RIGHT FORWARD 1
    PORTB |= (1 << PIN_IN4_FORWARD_RIGHT);
    // Right BACKWARD 0
    PORTB &= ~(1 << PIN_IN3_BACKWARD_RIGHT);
}

// Leichte Abbiegen nach links
void drive_right(){
    // Set the duty cycles for PD5/PD6
    //links
    setDutyCycle(PD5, 255);
    //rechts
    setDutyCycle(PD6, 155);
    // links  FORWARD, 1
    PORTD |= (1 << PIN_IN1_FORWARD_LEFT);
    // links BACKWARD 1
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    // RIGHT FORWARD 1
    PORTB |= (1 << PIN_IN4_FORWARD_RIGHT);
    // Right BACKWARD 0
    PORTB &= ~(1 << PIN_IN3_BACKWARD_RIGHT);
}

void gerade(){
    // Set the duty cycles for PD5/PD6, speed
    setDutyCycle(PD5, 155);
    setDutyCycle(PD6, 155);
    // Both sides stop, backward
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    PORTB &= ~(1 << PIN_IN3_BACKWARD_RIGHT);
    // Both sides forward
    PORTD |= (1 << PIN_IN1_FORWARD_LEFT);
    PORTB |= (1 << PIN_IN4_FORWARD_RIGHT);
}

void stop(){
	 // Both sides stop, backward & forward
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    PORTB &= ~(1 << PIN_IN3_BACKWARD_RIGHT);
    PORTD &= ~(1 << PIN_IN1_FORWARD_LEFT);
    PORTB &= ~(1 << PIN_IN4_FORWARD_RIGHT);
	}


void init_run(){
    // Set Data Direction Register C [0|1|2] as input.
    DDRC = ~((1 << DDC0) | (1 << DDC1) | (1 << DDC2));
    // Initialize U(S)ART!
    USART_init(UBRR_SETTING);
    // Delete everything on ports B and D
    DDRD = 0;
    DDRB = 0;
    // Set PD5 and PD6 as output (EN[A|B]!)
    DDRD = (1 << DD5) | (1 << DD6);
    // Set PD7 as output (IN1)
    DDRD |= (1 << PIN_IN1_FORWARD_LEFT);
    // Make PWM work on PD[5|6], immer fur motor
    setupTimer0();
    // Set PB0, PB1, and PB3 as output (IN[2|3|4])
    DDRB = (1 << DD0) | (1 << DD1) | (1 << DD3);
    // Set the duty cycles for PD5/PD6
    setDutyCycle(PD5, 155);
    setDutyCycle(PD6, 155);
}




int main(void){
	setup_heartbeat_timer();
	int start = 0;
	
    init_run();
    
    // ADC zu lesen   
    USART_init(UBRR_SETTING);

    DR_ADC0 &= ~(1 << DP_ADC0);
    DR_ADC1 &= ~(1 << DP_ADC1);
    DR_ADC2 &= ~(1 << DP_ADC2);

    ADC_init();

    unsigned char strbuff[sizeof(ADCMSG) + 15];

    uint16_t adcval0 = 0;
    uint16_t adcval1 = 0;
    uint16_t adcval2 = 0;
    
    while(1){
		
        adcval0 = ADC_read_avg(ADMUX_CHN_ADC0, ADC_AVG_WINDOW);
        adcval1 = ADC_read_avg(ADMUX_CHN_ADC1, ADC_AVG_WINDOW);
        adcval2 = ADC_read_avg(ADMUX_CHN_ADC2, ADC_AVG_WINDOW);
        
        int left = (adcval2 > 350);
        int right = (adcval0 > 300);
        int middle = (adcval1 > 200);
        
        if (!left && middle && !right) {
           gerade();
           start = 0;
        }
        else if (!left && !middle && !right) {
           //todo;
        }
        else if (!left && !middle && right) {
           drive_right();
            start = 0;

        }

        else if (!left && middle && right) {
           turn_right();
           start = 0;

        }
        else if(left && !middle && !right){
			drive_left();
			start = 0;

		}
		else if (left && !middle && right){
			//todo;
		}
		else if( left && middle && !right){
			turn_left();
			start = 0;

		}
		else if (left && middle && right){
			//if schwellwert dann hochzählen
			if (!start){
				start = m_second;
				}
			if(m_second-start > 100){
				stop();
				break;
				}

			
			
		}
        

    }
    return 0;
}
