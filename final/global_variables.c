#include "global_variables.h"

//for timer
uint16_t cnt = 0;
uint16_t m_second = 0;
char centi_second = 0;
char half_second = 0;
char second = 0;

//for run
int currentLap = 1;
time_t raceStartTime;
char isPaused = 0;
char isSessionActive = 0;
char isTurning = 0;
char isCompleted = 0;
