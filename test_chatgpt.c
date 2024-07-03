#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "iesusart.h"
#include "iesadc.h"
#include "iesmotors.h"

// Makros für Pins
#define PIN_IN1_FORWARD_LEFT PD7
#define PIN_IN2_BACKWARD_LEFT PB0
#define PIN_IN3_BACKWARD_RIGHT PB1
#define PIN_IN4_FORWARD_RIGHT PB3

#define LED_PORT PORTB
#define LED_DDR DDRB
#define LED_PIN PB5

uint16_t cnt = 0;
uint16_t m_second = 0;
uint8_t is_on_start_field = 0;
volatile uint8_t command_received = 0;
volatile uint8_t mode = 0; // 0 = Idle, 1 = Driving, 2 = Paused, 3 = Turning

ISR(TIMER1_COMPA_vect) {
    cnt += 1;
    if (cnt == 625) {
        cnt = 0;
        m_second += 1;
    }
}

ISR(USART_RX_vect) {
    char received = UDR0;
    if (received == 'S' || received == 'P' || received == 'T' || received == 'H' || received == 'E') {
        command_received = received;
    }
}

void setup_heartbeat_timer() {
    cli();                   // Interrupts deaktivieren
    TCCR1B |= (1 << CS10);   // Prescaler: 1 => 16E6 ticks/second
    TCCR1B |= (1 << WGM12);  // Timer 1 im CTC-Modus verwenden
    TIMSK1 |= (1 << OCIE1A); // Compare-Match-Interrupt für OCR1A aktivieren
    OCR1A = 255;             // Jeder 16E6/256 Ticks wird COMPA_vect ausgelöst
    sei();                   // Interrupts aktivieren
}

void setup_led() {
    LED_DDR |= (1 << LED_PIN); // Set LED pin as output
}

void toggle_led() {
    LED_PORT ^= (1 << LED_PIN); // Toggle LED state
}

// Leichte Abbiegen nach rechts
void turn_right() {
    setDutyCycle(PD5, 255); // links
    setDutyCycle(PD6, 180); // rechts
    PORTD |= (1 << PIN_IN1_FORWARD_LEFT); 
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    PORTB |= (1 << PIN_IN3_BACKWARD_RIGHT);
    PORTB &= ~(1 << PIN_IN4_FORWARD_RIGHT);
}

// Große Abbiegen nach links
void turn_left() {
    setDutyCycle(PD5, 180); // links
    setDutyCycle(PD6, 255); // rechts
    PORTD &= ~(1 << PIN_IN1_FORWARD_LEFT);
    PORTB |= (1 << PIN_IN2_BACKWARD_LEFT);
    PORTB |= (1 << PIN_IN4_FORWARD_RIGHT);
    PORTB &= ~(1 << PIN_IN3_BACKWARD_RIGHT);
}

// Leichte Abbiegen nach links
void drive_left() {
    setDutyCycle(PD5, 155); // links
    setDutyCycle(PD6, 255); // rechts
    PORTD |= (1 << PIN_IN1_FORWARD_LEFT);
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    PORTB |= (1 << PIN_IN4_FORWARD_RIGHT);
    PORTB &= ~(1 << PIN_IN3_BACKWARD_RIGHT);
}

// Leichte Abbiegen nach rechts
void drive_right() {
    setDutyCycle(PD5, 255); // links
    setDutyCycle(PD6, 155); // rechts
    PORTD |= (1 << PIN_IN1_FORWARD_LEFT);
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    PORTB |= (1 << PIN_IN4_FORWARD_RIGHT);
    PORTB &= ~(1 << PIN_IN3_BACKWARD_RIGHT);
}

void gerade() {
    setDutyCycle(PD5, 155);
    setDutyCycle(PD6, 155);
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    PORTB &= ~(1 << PIN_IN3_BACKWARD_RIGHT);
    PORTD |= (1 << PIN_IN1_FORWARD_LEFT);
    PORTB |= (1 << PIN_IN4_FORWARD_RIGHT);
}

void stop() {
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    PORTB &= ~(1 << PIN_IN3_BACKWARD_RIGHT);
    PORTD &= ~(1 << PIN_IN1_FORWARD_LEFT);
    PORTB &= ~(1 << PIN_IN4_FORWARD_RIGHT);
}

void init_run() {
    DDRC = ~((1 << DDC0) | (1 << DDC1) | (1 << DDC2));
    USART_init(UBRR_SETTING);
    DDRD = 0;
    DDRB = 0;
    DDRD = (1 << DD5) | (1 << DD6);
    DDRD |= (1 << PIN_IN1_FORWARD_LEFT);
    setupTimer0();
    DDRB = (1 << DD0) | (1 << DD1) | (1 << DD3);
    setDutyCycle(PD5, 155);
    setDutyCycle(PD6, 155);
    setup_led();
    USART_enable_interrupt();
}

void handle_start_field_detection() {
    if (is_on_start_field) {
        if (m_second % 2 == 0) {
            toggle_led();
            USART_transmitString("Same story, different student ... boring, IES needs to refactor this course.\n\n");
        }
    } else {
        if (m_second % 1 == 0) {
            toggle_led();
            USART_transmitString("Hey you, you know what to do. :-)\n\n");
        }
    }
}

void handle_command(char command) {
    if (command == 'S' && mode == 0) {
        // Start driving
        mode = 1;
        gerade();
        USART_transmitString("Here I am once more, going down the only round I’ve ever known...\n\n");
    } else if (command == 'P' && (mode == 1 || mode == 2)) {
        // Pause or unpause
        if (mode == 1) {
            mode = 2;
            stop();
            USART_transmitString("Paused ... zzzZZZzzzZZZzzz ... P again to unpause me.\n\n");
        } else {
            mode = 1;
            gerade();
            USART_transmitString("Unpaused ... let's go!\n\n");
        }
    } else if (command == 'T' && (mode == 1 || mode == 2)) {
        // Turn in clockwise direction
        mode = 3;
        turn_right();
        USART_transmitString("Lalalala!\n\n");
    } else if (command == 'H' && mode == 3) {
        // Stop turning
        mode = 1;
        stop();
        USART_transmitString("Stop turning ... back to track!\n\n");
    } else if (command == 'E' && mode == 0) {
        // Extended mode
        mode = 4;
        USART_transmitString("Extended mode activated.\n\n");
    }
    command_received = 0; // Reset command
}

int main(void) {
    setup_heartbeat_timer();
    int start = 0;

    init_run();

    DR_ADC0 &= ~(1 << DP_ADC0);
    DR_ADC1 &= ~(1 << DP_ADC1);
    DR_ADC2 &= ~(1 << DP_ADC2);

    ADC_init();

    unsigned char strbuff[sizeof(ADCMSG) + 15];

    uint16_t adcval0 = 0;
    uint16_t adcval1 = 0;
    uint16_t adcval2 = 0;

    while (1) {
        adcval0 = ADC_read_avg(ADMUX_CHN_ADC0, ADC_AVG_WINDOW);
        adcval1 = ADC_read_avg(ADMUX_CHN_ADC1, ADC_AVG_WINDOW);
        adcval2 = ADC_read_avg(ADMUX_CHN_ADC2, ADC_AVG_WINDOW);

        int left = (adcval2 > 350);
        int right = (adcval0 > 300);
        int middle = (adcval1 > 200);

        // Check if the robot is on the start field
        is_on_start_field = (adcval0 < 200 && adcval1 < 200 && adcval2 < 200);

        if (mode == 0) { // Idle mode
            handle_start_field_detection();
        } else if (mode == 1) { // Driving mode
            // Implement driving logic here
        } else if (mode == 2) { // Paused mode
            if (m_second % 2 == 0) {
                toggle_led();
            }
        } else if (mode == 3) { // Turning mode
            if (m_second % 1 == 0) {
                USART_transmitString("Lalalala!\n\n");
            }
        }

        if (command_received) {
            handle_command(command_received);
        }

        // Line following logic
        if (!left && middle && !right) {
            gerade();
            start = 0;
        } else if (!left && !middle && !right) {
            // TODO
        } else if (!left && !middle && right) {
            drive_right();
            start = 0;
        } else if (!left && middle && right) {
            turn_right();
            start = 0;
        } else if (left && !middle && !right) {
            drive_left();
            start = 0;
        } else if (left && !middle && right) {
            // TODO
        } else if (left && middle && !right) {
            turn_left();
            start = 0;
        } else if (left && middle && right) {
            // If threshold reached, stop the robot
            if (!start) {
                start = m_second;
            }
            if (m_second - start > 100) {
                stop();
                break;
            }
        }
    }
    return 0;
}
