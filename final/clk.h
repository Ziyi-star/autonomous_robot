#ifndef CLK_h
#define CLK_h


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <time.h>
#include "iesusart.h"

#define OR_HEARTBEAT PORTB
#define OP_HEARTBEAT PB5
#define DR_HEARTBEAT DDRB
#define DP_HEARTBEAT DDB5


//for timer
uint16_t cnt = 0;
uint16_t m_second = 0;

//for run
int currentLap = 0;
time_t raceStartTime;
int isPaused = 0;
int isSessionActive = 0;
int isTurning = 0;
int isCompleted = 0;



void setup_heartbeat_timer();

#endif
