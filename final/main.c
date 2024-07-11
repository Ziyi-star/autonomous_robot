#include "main.h"


// Define a structure to hold the ADC values
typedef struct {
    uint16_t adc0;
    uint16_t adc1;
    uint16_t adc2;
} ADCValues;


ADCValues initialAdcValues;


void check_adc_rotate() {
    uint16_t currentAdc0, currentAdc1, currentAdc2;
    // Loop until the break condition is met
    while (1) {
        // Read current ADC values
        currentAdc0 = ADC_read_avg(ADMUX_CHN_ADC0, ADC_AVG_WINDOW);
        currentAdc1 = ADC_read_avg(ADMUX_CHN_ADC1, ADC_AVG_WINDOW);
        currentAdc2 = ADC_read_avg(ADMUX_CHN_ADC2, ADC_AVG_WINDOW);

        // Check if current ADC values are close enough to initial values
        if (abs(currentAdc0 - initialAdcValues.adc0) < 30 &&
            abs(currentAdc1 - initialAdcValues.adc1) < 30 &&
            abs(currentAdc2 - initialAdcValues.adc2) < 30) {
            stop();  // Stop the robot
            break;
        } else {
            rotate_clockwise();  // Continue rotating
            _delay_ms(100);  // Delay to allow for ADC reading stabilization
        }
    } 
}


void handleDrivingLogic(int *left, int *middle, int *right, int *last_right, int *start) {
    if (!*left && *middle && !*right) {
        gerade();
        *start = 0;
    } else if (!*left && !*middle && !*right) {
        if (*last_right) {
            turn_right();
        } else {
            turn_left();
        }
    } else if (!*left && !*middle && *right) {
        drive_right();
        *start = 0;
    } else if (!*left && *middle && *right) {
        turn_right();
        *start = 0;
    } else if (*left && !*middle && !*right) {
        drive_left();
        *start = 0;
    } else if (*left && !*middle && *right) {
        gerade();
        *start = 0;
    } else if (*left && *middle && !*right) {
        turn_left();
        *start = 0;
    }
    // Start field
    else if (*left && *middle && *right) {
        gerade();
        // If threshold then increment
        if (!*start) {
            *start = m_second;
        }
        if (m_second - *start > 10) {
			isCompleted = 1;
            stop();
            currentLap++;
            return; 
        }
    }
    if (*left || *right) {
        if (*right) {
            *last_right = 1;
        } else {
            *last_right = 0;
        }
    }
}


int main(void) {
	
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

    while (1) {
		
		message = USART_receiveByte();
		 
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
        
		// S pressed and before first round
        if (message == 'S' && currentLap == 0) {
			isSessionActive = 1;
			handleDrivingLogic(&left, &middle, &right, &last_right, &start);        
        }
        
        // currentLap >=1
        if (isSessionActive && !isPaused) {
            if (isCompleted && currentLap == 1) {
				isCompleted = 0;
				USART_print("Here I am once more, going down the only round I've ever known...\n"); 
                handleDrivingLogic(&left, &middle, &right, &last_right, &start);  
            } 
            if (isCompleted && currentLap == 2) {
				isCompleted = 0;
				USART_print("YEAH, done first lap, feelig well, going for lap 2/3\n"); 
				handleDrivingLogic(&left, &middle, &right, &last_right, &start);  
            } 
            if (isCompleted && currentLap == 3) {
				isCompleted = 0;
				USART_print("YEAH YEAH, done 2nd lap, feeling proud, going for lap 3/3\n"); 
				handleDrivingLogic(&left, &middle, &right, &last_right, &start);  			
				}
			if (isCompleted && currentLap == 4) {
                int totalSeconds = (int)(time(NULL) - raceStartTime);
                USART_print(" Finally finished , It's over and done now, after $SECONDS seconds. Thanks for working with me! :-) I will reset myself in 5 seconds. Take care!\n");
                //todo: reset
                isSessionActive = 0;
                break;
            }
        }
        
         // Additional logic for pause and resume
        if (message == 'P') {
            isPaused = !isPaused;
            if (isPaused) {
                stop();  // Stop any movement
                run_led_sequence(regmdl,500);
            }
        }

        // Turning logic
        if (message == 'T' && (isSessionActive || isPaused)) {
            isTurning = 1;
			// Store current ADC values for 'H'
			initialAdcValues.adc0 = adcval0;
			initialAdcValues.adc1 = adcval1;
			initialAdcValues.adc2 = adcval2;
            rotate_clockwise();
        } 
        else if (isTurning && message == 'H') {
            isTurning = 0;
            // Ensure the robot stops rotating
            stop();  
			// Function to handle the rotation until ADC values match
			check_adc_rotate();  
        }
    }
    return 0;
}
