/**
 * @file main.c
 * @author Ziyi Liu
 * @date 04.09.2024
 *
 * @brief Main Control Loop for Autonomous Robotic Navigation
 * 
 * @details This file contains the main function which serves as the central control hub for an autonomous robot.
 *          It integrates sensor data processing, state management, and command execution to handle different
 *          operational modes such as navigation, obstacle detection, and system diagnostics. The robot's behavior
 *          is influenced by ADC values to dynamically adjust its course and by UART messages to switch between
 *          different operational states.
 * 
 *  
 */


#include "main.h"

/**
 * @struct ADCValues
 * @brief Holds ADC (Analog-to-Digital Converter) values from multiple channels.
 *
 * @details This structure is designed to store the digital values converted from analog signals
 * on multiple ADC channels. It can be used for easy access and manipulation of these values 
 * within the application.
 *
 * @var ADCValues::adc0
 * uint16_t adc0: ADC value from channel 0.
 * @var ADCValues::adc1
 * uint16_t adc1: ADC value from channel 1.
 * @var ADCValues::adc2
 * uint16_t adc2: ADC value from channel 2.
 */
typedef struct {
    uint16_t adc0;
    uint16_t adc1;
    uint16_t adc2;
} ADCValues;


ADCValues initialAdcValues;

int main(void) {
	time_counter = 0;
	count_time = 0;
	
    setup_heartbeat_timer();
    int start = 0;
    
    init_run();
   
    USART_init(UBRR_SETTING);
    
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
    
    int counted_startfield = 1;
    int last_right = 0;
    char message = 0;
    char str_buffer [100];
    char state = 0;

    while (1) {

		// Regularly Kick (Reset) the Watchdog
    	wdt_reset();
		
		message = USART_receiveByte();
		
        adcval0 = ADC_read_avg(ADMUX_CHN_ADC0, ADC_AVG_WINDOW);
        adcval1 = ADC_read_avg(ADMUX_CHN_ADC1, ADC_AVG_WINDOW);
        adcval2 = ADC_read_avg(ADMUX_CHN_ADC2, ADC_AVG_WINDOW);
        
        left = (adcval2 > 350);
        right = (adcval0 > 300);
        middle = (adcval1 > 300);
        
        update_model(regmdl, left, middle, right);
		
		//Um Buchstaben zu handeln
		//STARTFIELD
		if(message == 'S' && left && middle && right && state != 'S'){
			//einmalig Mode ändern zu 'S'
			state = 'S';
			USART_print("Here I am once more, going down the only round I've ever known...\n"); 
			count_time = 1;
		} 
		//'P' gedrückt
		if(message == 'P'){
			if(state == 'P'){
				state = 'S';
			}
			if(state == 'S'){
				state = 'P';
			}
		
		} 
		//'T' gedrückt
		if(message == 'T'){
			// vorher fahren 'S' mode, dann wechsel zu trabble 'T' mode
			if(state == 'S'){
				state = 'T';
			}
		
		} 
		// 'H' gedrückt
		if(message == 'H'){
			// vorher trabble 'T' mode, 
			if(state == 'T'){
				state = 'H';
			}
		} 
		
		
		//NOT STARTFIELD
		if(state == 0 && !(middle && right && left) ){
			if (centi_second) {
				USART_print("Hey you, you know what to do. :-)\n");
				centi_second = 0;
			}
		}
		
		//LED goes with the adc
        if (*regmdl != last_model_state) {
            update_hardware(regmdl);
            last_model_state = *regmdl;
        }

		// Fahren 'S' mode 
        if (state == 'S'){
			//run logic
			if (middle) {
				gerade();
			}
			else if (right) {
				small_right();

			}
			else if(left){
				small_left();

			}
			//um die Ecke
			else if (!left && !middle && !right) {
				if (last_right){
					big_right();
					}
				else{
					big_left();
					}
				}
			//startfield
			if (left && middle && right){
				//if schwellwert dann hochzählen
				if (!start){
					start = m_second;
					}
				if(m_second-start > 5 && counted_startfield == 0){
					counted_startfield = 1;
					currentLap++;
					if (currentLap == 2){
					USART_print("YEAH, done first lap, feelig well, going for lap 2/3\n");
					}
					if (currentLap == 3){
					USART_print("YEAH YEAH, done 2nd lap, feeling proud, going for lap 3/3\n"); 
					}
					if (currentLap == 4) {
						//TODO: reset?
						sprintf(str_buffer, " Finally finished , It's over and done now, after #%d seconds. Thanks for working with me! :-) I will reset myself in 5 seconds. Take care!\n", time_counter);
						USART_print(str_buffer);
						stop();
						_delay_ms(5000);
						wdt_enable(WDTO_15MS);
						// Loop forever, watchdog will reset the microcontroller
						while(1);  
					}
				}
			} else{
				counted_startfield = 0;
				start = 0;
			}
			
			if (left || right){
				if (right){
					last_right = 1;
				}
				else{
					last_right = 0;
				}
			}
					
			//print 1hz things
			if (second) {
				sprintf(str_buffer, "Currently I go round #%d\n", currentLap);
				USART_print(str_buffer);
				second = 0;
			}
				
		}
		
		if (state == 'P'){
			stop();  
			run_led_sequence(regmdl);
			if (second){
				USART_print("Paused ... zzzZZZzzzZZZzzz ... P again to unpause me.\n");
				second = 0;
			}
			while(1){
				message = USART_receiveByte();
				if(message=='P'){
					state='S';
					break;
				}
			}
		}
			
		if (state == 'T'){
			// Store current ADC values for 'H'
			initialAdcValues.adc0 = adcval0;
			initialAdcValues.adc1 = adcval1;
			initialAdcValues.adc2 = adcval2;
			rotate_clockwise();
			if (half_second){
				USART_print("lalala.\n");
				half_second = 0;
			}
		}

		if (state == 'H'){
			stop();  
			// handle the rotation until ADC values match
			uint16_t currentAdc0, currentAdc1, currentAdc2;
			// Loop until the break condition is met
			while (1) {
				// Read current ADC values
				currentAdc0 = ADC_read_avg(ADMUX_CHN_ADC0, ADC_AVG_WINDOW);
				currentAdc1 = ADC_read_avg(ADMUX_CHN_ADC1, ADC_AVG_WINDOW);
				currentAdc2 = ADC_read_avg(ADMUX_CHN_ADC2, ADC_AVG_WINDOW);

				// Rotate for the duration specified
				int duration_ms = 10;
				while (duration_ms > 0) {
					_delay_ms(1);  // Delay in milliseconds
					duration_ms--; // Decrement the counter
					rotate_clockwise();  // Continue rotating
				}
				if (currentAdc0 > 0 || currentAdc1 > 0 || currentAdc2 > 0){
					state = 'S';
					break;
				}
			}

		}
	}
	return 0;      
}