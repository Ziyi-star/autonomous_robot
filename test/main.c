#include <avr/io.h>
#include "track.h"
#include "iesadc.h"
#include "led.h"



int main(void) {
	
    init_run();
    
	// ADC zu lesen 
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

    // Allocate 1 byte in memory/on heap for a representation (model) of the register and clear the contents directly, 
    // and update everything accordingly.
    srr_t *regmdl = malloc(sizeof(srr_t));
    clear(regmdl);

    srr_t last_model_state = *regmdl;
    
    
    int left;
    int right;
    int middle;
    
    int last_right = 0;
    char message = 0;
    char str_buffer [50];

    while (1) {
				 
        adcval0 = ADC_read_avg(ADMUX_CHN_ADC0, ADC_AVG_WINDOW);
        adcval1 = ADC_read_avg(ADMUX_CHN_ADC1, ADC_AVG_WINDOW);
        adcval2 = ADC_read_avg(ADMUX_CHN_ADC2, ADC_AVG_WINDOW);
        
        left = (adcval2 > 350);
        right = (adcval0 > 300);
        middle = (adcval1 > 200);
        
        update_model(regmdl, left, middle, right);
		
		//LED goes with the adc
        if (*regmdl != last_model_state) {
            update_hardware(regmdl);
            last_model_state = *regmdl;
        }
        
        //start
        if (left && middle && right){
			gerade();
		}	
			
		else if (!left && middle && !right) {
		   gerade();
	
		}
		else if (!left && !middle && !right) {
		   if (last_right){
			   big_right();
			   }
			else{
				big_left();
				}
		}
		else if (!left && !middle && right) {
		   big_right();
			

		}
		else if (!left && middle && right) {
		   small_right();
		  

		}
		else if(left && !middle && !right){
			big_left();
			

		}
		else if (left && !middle && right){
			gerade();
		}
		else if( left && middle && !right){
			small_left();
			
		}
		//startfield
		
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
    
