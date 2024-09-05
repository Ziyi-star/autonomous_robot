/**
 * @file main.h
 * @author Ziyi Liu
 * @date 04.09.2024
 *
 * @brief Main Control Loop for Autonomous Robotic Navigation
 * 
 * @details This file contains the main function which serves as the central control hub for an autonomous robot.
 *          It integrates sensor data processing, state management, and command execution to handle different
 *          operational modes such as navigation, obstacle detection, and system diagnostics. The robot's behavior
 *          is influenced by ADC values to dynamically adjust its course and by UART messages to switch between
 *          different operational states.
 * 
 *  
 */


#include <avr/io.h>
#include <avr/wdt.h>
#include "clk.h"
#include "led.h"
#include "track.h"
#include "iesusart.h"
#include "iesadc.h"
#include "config.h"
