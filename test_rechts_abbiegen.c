#include "iesmotors.h"
#include <util/delay.h>


int main(void) {
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
    
    while(1){
	// Right motors forward, starting with PWM=0% and slowly increas the duty
    setDutyCycle(PD6, 155);
    PORTB |= (1 << PB3);



    return 0;
		
		}
	}
