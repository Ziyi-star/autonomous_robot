#ifndef GLOBALS_H
#define GLOBALS_H
#include <time.h>

//for timer
extern uint16_t cnt;
extern uint16_t m_second;
extern char centi_second;
extern char half_second;
extern char second;

//for run
extern int currentLap;
extern time_t raceStartTime;
extern char isPaused;
extern char isSessionActive;
extern char isTurning;
extern char isCompleted;

#endif
