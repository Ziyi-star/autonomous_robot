/**
 * @author Ziyi Liu
 * @date 04.09.2024
 * @file global_variables.h
 * @brief Global variables for timing and race management.
 *
 * @details This header file declares external global variables used across multiple modules for handling
 * timing functions and race state management in a racing application. These variables are crucial
 * for tracking the time and state of race events.
 */


#ifndef GLOBALS_H
#define GLOBALS_H
#include <time.h>

//for timer
extern uint16_t cnt;        /**< Counter for low-level timing operations. */
extern uint16_t m_second;   /**< Milliseconds counter. */
extern char centi_second;    /**< Centiseconds counter. */
extern char half_second;     /**< Half-second flag, set every half second. */
extern char second;         /**< Second flag, set every second. */

//for run
extern int currentLap;              /**< Current lap number in the race. */
extern uint16_t time_counter;        /**< Generic time counter for various uses. */
extern char count_time;             /**< Flag to indicate whether to count time or not. */

#endif
