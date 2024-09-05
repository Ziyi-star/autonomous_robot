/**
 * @file led.c
 * @author Ziyi Liu
 * @date 04.09.2024
 * @brief LED Control for Robotic Shift Register Interface
 *
 * @details This file includes functions that interface with the robot's shift register to control
 *          LEDs based on specific patterns and timing. Functions provide mechanisms to clock the
 *          hardware, update LED states both in memory and on the actual hardware, clear settings,
 *          and execute LED sequences to represent various states or signals visually.
 * 
 *  
 */


#include "led.h"

/**
  Clocks the real hardware -- whenever this is called,
  a rising edge on PD4 is generated.
*/
void clk() {
    OR_SR_CLK &= ~(1 << OP_SR_CLK);
    OR_SR_CLK |= (1 << OP_SR_CLK);
    // There may be dragons in here. How long has a
    // clock pulse to be high? How do you know?
    OR_SR_CLK &= ~(1 << OP_SR_CLK);
}

/**
  Writes the in-memory-representation (the "model") of
  the robot's shift-register to the real hardware.
*/
void update_hardware(srr_t *regmdl) {
    for(cntv8_t i = 0; i < REGWIDTH; i++) {
        unsigned char position_set = (*regmdl & (1 << i));
        if (position_set) {
           OR_SR_DATA |= (1 << OP_SR_DATA);
     
        } else {
           OR_SR_DATA &= ~(1 << OP_SR_DATA);
        }
        clk();
    }
}

/**
  Updates the model (memory), that represents the
  robot's shift-register.
*/
void update_model(srr_t *regmdl, int left, int middle, int right) {

    if (right) {
        *regmdl |= (1 << SR_LED_YELLOW);
    } else {
        *regmdl &= ~(1 << SR_LED_YELLOW);
    }

    if (middle) {
        *regmdl |= (1 << SR_LED_GREEN);
    } else {
        *regmdl &= ~(1 << SR_LED_GREEN);
    }

    if (left) {
        *regmdl |= (1 << SR_LED_BLUE);
    } else {
        *regmdl &= ~(1 << SR_LED_BLUE);
    }

}

/**
  Clears the in-memory-representation (the "model") of
  the robot's shift-register.
*/
void clear(srr_t *regmdl) {
    *regmdl = 0;
    update_hardware(regmdl);
}

/**
  Sets data directions.
*/
void setup_ddr_all() {
    // Set Data Direction Register B2 as output.
    DR_SR_DATA |= 1 << DP_SR_DATA;

    // Set Data Direction Register D4 as output. To D4,
    // the CLK-line of the robot's shift-register is connected.
    DR_SR_CLK |= 1 << DP_SR_CLK;

    // Set Data Direction Register C[0|1|2] as input. To these pins,
    // the robot's line-infrared-reflection-sensors are attached.
    DR_LF_L &= ~(1 << DP_LF_L);
    DR_LF_R &= ~(1 << DP_LF_R);
    DR_LF_M &= ~(1 << DP_LF_M);
}


/**
  Function to run LED sequence like a running light.
*/

void run_led_sequence(srr_t *regmdl) {
    const int delay_ms = 143; // Each LED lights up for 143 ms

    // Forward direction
    for (int i = 0; i < REGWIDTH; i++) {
        *regmdl = 1 << i;  // Turn on only the ith LED
        update_hardware(regmdl);
        _delay_ms(delay_ms);
    }

    // Backward direction, skipping the first and last LEDs as they are already handled
    for (int i = REGWIDTH - 2; i > 0; i--) {
        *regmdl = 1 << i;
        update_hardware(regmdl);
        _delay_ms(delay_ms);
    }

    // Clear LEDs at the end of the cycle
    clear(regmdl);
}


