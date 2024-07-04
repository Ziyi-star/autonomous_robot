#include "test_main.h"


uint16_t cnt = 0;
uint16_t m_second = 0;
uint16_t ride = 0;

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
    
        // Setup everything
    setup_ddr_all();
    USART_init(UBRR_SETTING);

    // Allocate 1 byte in memory/on heap for a representation (model)
    // of the register and clear the contents directly, and update
    // everything accordingly.
    srr_t *regmdl = malloc(sizeof(srr_t));
    clear(regmdl);

    srr_t last_model_state = *regmdl;
    
        int left;
        int right;
        int middle;
        
        char last_right = 0;
        
        char message = 0;
        

    while (1) {
   
		
        adcval0 = ADC_read_avg(ADMUX_CHN_ADC0, ADC_AVG_WINDOW);
        adcval1 = ADC_read_avg(ADMUX_CHN_ADC1, ADC_AVG_WINDOW);
        adcval2 = ADC_read_avg(ADMUX_CHN_ADC2, ADC_AVG_WINDOW);
        
        left = (adcval2 > 350);
        right = (adcval0 > 300);
        middle = (adcval1 > 200);
        
        
        update_model(regmdl, left, middle, right);

        if (*regmdl != last_model_state) {
           update_hardware(regmdl);
           last_model_state = *regmdl;
        }
        
        if (message == 'S’){
        //TODO: alle Fahrlogik
        
        
			
			if (message == 'P'){
				//TODO: 1. tes mal stop, Ampel blicken, Sachen ausgeben
				
				// TODO: 2.tes mal, break, draußen gehen
				}
			
			if (message == 'T'){
				//TODO: Urzeigesinn drehen, Sachen ausgeben
				
				if (message == 'H'){
					//TODO: break, in die ursprüngliche RIchtung

					
					}
				}
			
			}
			
		
        
        if (!left && middle && !right) {
           gerade();
           start = 0;
        }
        else if (!left && !middle && !right) {
           if (last_right){
			   turn_right();
			   }
			else{
				turn_left();
				}
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
		//startfield
		else if (left && middle && right){
			gerade();
			//if schwellwert dann hochzählen
			if (!start){
				start = m_second;
				}
			if(m_second-start > 10){
				stop();
				ride = ride + 1;
				break;
				}
			
		}
		if (left || right){
			if (right){
				last_right = 1;
				}
			else{
				last_right = 0;
				}
			}
        

    }
    return 0;
}
