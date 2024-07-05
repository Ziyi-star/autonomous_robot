#ifndef LED_h
#define LED_h

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "iesusart.h"

// Robot function/peripheral RIGHT LF.
#define DR_LF_R DDRC
#define DP_LF_R DDC0
#define IR_LF_R PINC
#define IP_LF_R PINC0

// Robot function/peripheral MIDDLE LF.
#define DR_LF_M DDRC
#define DP_LF_M DDC1
#define IR_LF_M PINC
#define IP_LF_M PINC1

// Robot function/peripheral LEFT LF.
#define DR_LF_L DDRC
#define DP_LF_L DDC2
#define IR_LF_L PINC
#define IP_LF_L PINC2

// Robot funktion/peripheral SR
#define REGWIDTH 3

// SR clock
#define DR_SR_CLK  DDRD
#define DP_SR_CLK  DDD4
#define OR_SR_CLK  PORTD
#define OP_SR_CLK  PORTD4

// SR data
#define DR_SR_DATA DDRB
#define DP_SR_DATA DDB2
#define OR_SR_DATA PORTB
#define OP_SR_DATA PORTB2

#define SR_LED_YELLOW 2
#define SR_LED_GREEN  1
#define SR_LED_BLUE   0

typedef unsigned char srr_t;
typedef unsigned char cntv8_t;


void clk();

void update_hardware(srr_t *regmdl);

void update_model(srr_t *regmdl, int left, int middle, int right);

void clear(srr_t *regmdl);

void setup_ddr_all();

void run_led_sequence(srr_t *regmdl, int delay_ms) ;

#endif
