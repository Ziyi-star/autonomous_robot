/**
 * @file track.h
 * @author Ziyi Liu
 * @date 04.09.2024
 * @brief Motor control definitions and functions for robotic movements.
 *
 * @details This header file contains macros and functions for controlling the directions and movements
 * of a robot's motors. It provides a high-level interface for common maneuvers such as turning and stopping.
 */

#ifndef TRACK_h
#define TRACK_h


#include <avr/io.h>
#include "iesusart.h"
#include "iesmotors.h"
#include "iesadc.h"

// Constants for PB and PD pins used in motor control
#define PIN_IN1_FORWARD_LEFT PD7        /**< Pin for forward movement on the left motor. */
#define PIN_IN2_BACKWARD_LEFT PB0       /**< Pin for backward movement on the left motor. */
#define PIN_IN3_BACKWARD_RIGHT PB1       /**< Pin for backward movement on the right motor. */
#define PIN_IN4_FORWARD_RIGHT PB3       /**< Pin for forward movement on the right motor. */


/**
 * @brief Turns the robot sharply to the right.
 */
void big_right();

/**
 * @brief Turns the robot sharply to the left.
 */
void big_left();

/**
 * @brief Turns the robot slightly to the right.
 */
void small_right();

/**
 * @brief Turns the robot slightly to the left.
 */
void small_left();

/**
 * @brief Moves the robot straight forward.
 */
void gerade();

/**
 * @brief Initializes the robot's motors for a run.
 */
void init_run();

/**
 * @brief Stops all motor movements of the robot.
 */
void stop();

/**
 * @brief Rotates the robot clockwise.
 */
void rotate_clockwise();


#endif



