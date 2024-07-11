#ifndef CLK_h
#define CLK_h


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <time.h>
#include "iesusart.h"
#include "global_variables.h"

#define OR_HEARTBEAT PORTB
#define OP_HEARTBEAT PB5
#define DR_HEARTBEAT DDRB
#define DP_HEARTBEAT DDB5


void setup_heartbeat_timer();

#endif
