#include <avr/io.h>
#include "iesusart.h"
#include "iesmotors.h"
#include "iesadc.h"


void turn_right(){
	// Set the duty cycles for PD5/PD6
    setDutyCycle(PD5, 200);
    setDutyCycle(PD6, 200);
	// links rückwärts
    PORTB &= ~(1 << PB0);
	//Right motors BACKWARD
    PORTB |= (1 << PB1);

	PORTB &= ~(1 << PB3);
	//links Motor rückwärt
	PORTD |= (1 << PD7);
	


		}

void drive_left(){
	// Set the duty cycles for PD5/PD6
    setDutyCycle(PD5, 155);
    setDutyCycle(PD6, 155);
	// Both sides stop, backword
    PORTB &= ~(1 << PB0);
    PORTB &= ~(1 << PB1);
    
	PORTB |= (1 << PB3);
	//links Motor 0 setzen
	PORTD &= ~(1 << PD7);
	}

void turn_left(){
	// Set the duty cycles for PD5/PD6
    setDutyCycle(PD5, 200);
    setDutyCycle(PD6, 200);
	// links rückwärts
    PORTB |= (1 << PB0);
	//Right motors BACKWARD
    PORTB &= ~(1 << PB1);
    
	PORTB |= (1 << PB3);
	//links Motor rückwärt
	PORTD &= ~(1 << PD7);

	
	}


void drive_right(){
	    // Set the duty cycles for PD5/PD6
    setDutyCycle(PD5, 155);
    setDutyCycle(PD6, 155);
	// Both sides stop, backword
    PORTB &= ~(1 << PB0);
    PORTB &= ~(1 << PB1);
	
	// Left motors forward, starting with PWM=0% and slowly increas the duty
	setDutyCycle(PD5, 155);
	PORTD |= (1 << PB7);
	// rechts Motor 0 setzen
	PORTB &= ~(1 << PB3);
	
	}

void gerade(){
	// Set the duty cycles for PD5/PD6
    setDutyCycle(PD5, 155);
    setDutyCycle(PD6, 155);
	 // Both sides stop, backword
    PORTB &= ~(1 << PB0);
    PORTB &= ~(1 << PB1);

	// Both sides forward
	PORTD |= (1 << PD7);
	PORTB |= (1 << PB3);
		}

int main(void) {
	
    // Set Data Direction Register C [0|1|2| as input.
    DDRC = ~((1<<DDC0) | (1<<DDC1) | (1<<DDC2));
    
    // Initialze U(S)ART!
    USART_init(UBRR_SETTING);
    
    // Delete everything on ports B and D
    DDRD = 0;
    DDRB = 0;

    // Set PD5 and PD6 as output (EN[A|B]!)
    DDRD = (1 << DD5) | (1 << DD6);

    // Set PD7 as output (IN1)
    DDRD |= (1 << DD7);

    // Make PWM work on PD[5|6], immer fue motor
    setupTimer0();

    // Set PB0, PB1, and PB3 as output (IN[2|3|4])
    DDRB = (1 << DD0) | (1 << DD1) | (1 << DD3);

    // Set the duty cycles for PD5/PD6
    setDutyCycle(PD5, 155);
    setDutyCycle(PD6, 155);

    //adc zu lesen   
    USART_init(UBRR_SETTING);

    DR_ADC0 &= ~(1 << DP_ADC0);
    DR_ADC1 &= ~(1 << DP_ADC1);
    DR_ADC2 &= ~(1 << DP_ADC2);

    ADC_init();

    unsigned char strbuff[sizeof(ADCMSG) + 15]; // WTF, why + 15? Oo

    // Just to make things clear: You have to be extremely careful with
    // the size of the stringbuffer. Better safe than sorry! But memory
    // as well as time are so so so precious!

    uint16_t adcval0 = 0;
    uint16_t adcval1 = 0;
    uint16_t adcval2 = 0;
    
    while(1) {
        adcval0 = ADC_read_avg(ADMUX_CHN_ADC0, ADC_AVG_WINDOW);
        adcval1 = ADC_read_avg(ADMUX_CHN_ADC1, ADC_AVG_WINDOW);
        adcval2 = ADC_read_avg(ADMUX_CHN_ADC2, ADC_AVG_WINDOW);

        //sprintf(strbuff, ADCMSG, adcval0, adcval1, adcval2);

        //USART_print(strbuff);
        
		//MIDDLE
        if (adcval1>280) {
           gerade();
        }
        //rechts
        else if (adcval0>400) {
			turn_right();
        }

        //LINKS
        else if (adcval1>240) {
			turn_left();
	
        }
        
        
    }
     return 0;
    
}



        
        
               
    
   
