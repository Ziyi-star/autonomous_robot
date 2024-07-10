#include "main.h"
#include "clk.h"
#include "led.h"
#include "track.h"

// Define a structure to hold the ADC values
typedef struct {
    uint16_t adc0;
    uint16_t adc1;
    uint16_t adc2;
} ADCValues;

int cnt_run = 0;
uint16_t m_second = 0;
ADCValues initialAdcValues;

int currentLap = 0;
time_t raceStartTime;
int lapCompleted = 0;
int isPaused = 0;
int isSessionActive = 0;
int isTurning = 0;
int isCompleted = 0;

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

// Function to print a specified message and delay for a specified amount of time
void pause_print(const char* message, int delayTime) {
    USART_print(message);
    _delay_ms(delayTime);
}

void handleDrivingLogic(int *left, int *middle, int *right, int *last_right, int *start, int *ride, uint16_t m_second) {
    if (!left && middle && !right) {
        gerade();
        *start = 0;
    } else if (!left && !middle && !right) {
        if (*last_right) {
            turn_right();
        } else {
            turn_left();
        }
    } else if (!left && !middle && right) {
        drive_right();
        *start = 0;
    } else if (!left && middle && right) {
        turn_right();
        *start = 0;
    } else if (left && !middle && !right) {
        drive_left();
        *start = 0;
    } else if (left && !middle && right) {
        gerade();
        *start = 0;
    } else if (left && middle && !right) {
        turn_left();
        *start = 0;
    }
    // Start field
    else if (left && middle && right) {
        gerade();
        // If threshold then increment
        if (!*start) {
            *start = m_second;
        }
        if (m_second - *start > 10) {
			isCompleted = 1
			if (currentLap == 0){
				//todo: 1Hz: Same story, different student ... boring, IES needs to refactor this course.
				}
            //stop();
            currentLap++;
            break;
        }
    }
    if (left || right) {
        if (right) {
            *last_right = 1;
        } else {
            *last_right = 0;
        }
    }
}

int main(void) {
	
    setup_heartbeat_timer();
    int start = 0;
    int ride = 0;
    
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

    // Allocate 1 byte in memory/on heap for a representation (model) of the register and clear the contents directly, and update everything accordingly.
    srr_t *regmdl = malloc(sizeof(srr_t));
    clear(regmdl);

    srr_t last_model_state = *regmdl;
    
    int left;
    int right;
    int middle;
    
    int last_right = 0;
    char message = 0;

    while (1) {
		
		message = USART_receiveByteSelf();
		 
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
           //todo:10Hz Hey you, you know what to do. :-)
           //todo: in handle put this string: 
           handleDrivingLogic(&left, &middle, &right, &last_right, &start, &ride, m_second,);        
        }
        
        // currentLap >=1
        if (isSessionActive && !isPaused) {
            if (isCompleted && currentLap == 1) {
				isCompleted = 0;
				//todo: einmalig: Here I am once more, going down the only round I've ever known...
                //todo: 1Hz Currently I go round #$ROUND
                //currentLap++;
                //runlogik
            } 
            if (isCompleted && currentLap == 2) {
				isCompleted = 0;
				//todo: einmalig:YEAH, done first lap, feelig well, going for lap 2/3
				//todo: 1Hz Currently I go round #$ROUND
                //currentLap++;
                //runlogik
            } 
            if (isCompleted && currentLap == 3) {
				isCompleted = 0;
				//todo:einmalig, YEAH YEAH, done 2nd lap, feeling proud, going for lap 3/3
				//todo: 1Hz Currently I go round #$ROUND
				//runlogik
			}
			if (checkLapComplete() && currentLap == 4) {
                int totalSeconds = (int)(time(NULL) - raceStartTime);
                char finalMsg[150];
                sprintf(finalMsg, "Finally finished, It's over and done now, after %d seconds. Thanks for working with me! :-) I will reset myself in 5 seconds. Take care!\n", totalSeconds);
                sendOneTimeMessage(finalMsg);
                //todo: reset
                break;
            }
        }
        
         // Additional logic for pause and resume
        if (message == 'P') {
            isPaused = !isPaused;
            if (isPaused) {
                stop();  // Stop any movement
                run_led_sequence(regmdl,500);
                pause_print("Paused ... zzzZZZzzzZZZzzz ... Press P again to unpause me\n", 1000);
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
            pause_print("Lalalala!\n", 500);
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
}
