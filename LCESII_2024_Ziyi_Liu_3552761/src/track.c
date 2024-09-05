/**
 * @file track.c
 * @author Ziyi Liu
 * @date 04.09.2024
 * @brief Motor Control Functions for Robotic Navigation
 *
* @details This file contains functions to control the movement of a robot through its motor-driven wheels. 
 *          Functions in this file allow for precise movement control, including sharp turns, slight adjustments,
 *          and straightforward motion. The control logic includes speed adjustment, direction control, and
 *          stopping mechanisms, integrating directly with the robot's hardware interface.
 */



#include "track.h"

void big_right(){
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


void big_left(){
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

void small_right(){
    //links
    setDutyCycle(PD5, 155);
    //rechts
    setDutyCycle(PD6, 0);
    // links  FORWARD, 1
    PORTD |= (1 << PIN_IN1_FORWARD_LEFT);
    // links BACKWARD 0
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    // RIGHT FORWARD 1
    PORTB |= (1 << PIN_IN4_FORWARD_RIGHT);
    // Right BACKWARD 0
    PORTB &= ~(1 << PIN_IN3_BACKWARD_RIGHT);
}


void small_left(){
    //links
    setDutyCycle(PD5, 0);
    //rechts
    setDutyCycle(PD6, 155);
    // links  FORWARD, 0
    PORTD &= ~(1 << PIN_IN1_FORWARD_LEFT);
    // links BACKWARD 1
    PORTB |= (1 << PIN_IN2_BACKWARD_LEFT);
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
    DDRC &= ~((1 << DDC0) | (1 << DDC1) | (1 << DDC2));
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
    DDRB |= (1 << DD0) | (1 << DD1) | (1 << DD3);
    // Set the duty cycles for PD5/PD6
    setDutyCycle(PD5, 155);
    setDutyCycle(PD6, 155);
}

#include "track.h"

// Schnelle Drehung im Uhrzeigersinn, right
void rotate_clockwise() {
    // Set the duty cycles for maximum differential speed
    //links
    setDutyCycle(PD5, 255);  // Maximum speed forward for left motors
    //rechts
    setDutyCycle(PD6, 0);    // Stop right motors or set to 0 for minimum speed if reverse not desired
    // links  FORWARD, 1
    PORTD |= (1 << PIN_IN1_FORWARD_LEFT);
    // links BACKWARD 1
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    // RIGHT FORWARD 1
    PORTB &= ~(1 << PIN_IN4_FORWARD_RIGHT);
    // Right BACKWARD 0
    PORTB &= ~(1 << PIN_IN3_BACKWARD_RIGHT);
}

// Schnelle Drehung im Uhrzeigersinn, right
void rotate_clockwise_oppsite() {
    // Set the duty cycles for maximum differential speed
    //links
    setDutyCycle(PD5, 0);  // Maximum speed forward for left motors
    //rechts
    setDutyCycle(PD6, 255);    // Stop right motors or set to 0 for minimum speed if reverse not desired
    // links  FORWARD, 1
    PORTD &= ~(1 << PIN_IN1_FORWARD_LEFT);
    // links BACKWARD 1
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    // RIGHT FORWARD 1
    PORTB |= (1 << PIN_IN4_FORWARD_RIGHT);
    // Right BACKWARD 0
    PORTB &= ~(1 << PIN_IN3_BACKWARD_RIGHT);
}






