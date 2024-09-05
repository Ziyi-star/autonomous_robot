/**
 * @file clk.h
 * @author Ziyi Liu
 * @date 04.09.2024
 * @brief Timer and Clock Management for Microcontroller
 *
* @details This header file contains definitions and functions for managing timers and clocks on AVR microcontrollers. 
 * It provides macros for port and pin definitions used in heartbeat signaling and includes essential AVR libraries 
 * to access hardware features. The setup_heartbeat_timer function declared in this file configures a timer used for 
 * generating periodic interrupts to toggle an LED as a heartbeat indicator, which helps in monitoring the system's health.
 * 
  */


#ifndef CLK_h
#define CLK_h


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <time.h>
#include "iesusart.h"
#include "config.h"

#define OR_HEARTBEAT PORTB
#define OP_HEARTBEAT PB5
#define DR_HEARTBEAT DDRB
#define DP_HEARTBEAT DDB5


void setup_heartbeat_timer();

#endif
