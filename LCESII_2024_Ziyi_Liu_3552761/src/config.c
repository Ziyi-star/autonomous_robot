/**
 * @file config.c
 * @author Ziyi Liu
 * @date 04.09.2024
 * @brief Global variables for timing and race control.
 *
 * @details This file defines global variables used across the project for managing timing functions 
 *          and race state in a robotic navigation system. These variables facilitate synchronization 
 *          and state management throughout various components of the system.
 */


#include "config.h"

//for timer
uint16_t cnt = 0;
uint16_t m_second = 0;
char centi_second = 0;
char half_second = 0;
char second = 0;

//for run
int currentLap = 1;
uint16_t time_counter = 0;
char count_time = 0;
