/*
 * Application_FSM.c
 *
 *  Created on: Aug 8, 2024
 *      Author: ubuntu
 */

/* Includes ------------------------------------------------------------------*/

#include "Application_FSM.h"

/* Private types -------------------------------------------------------------*/

typedef enum {
	STATE_IDLE, STATE_READ_SENSOR, STATE_CONTROL, STATE_ALARM
} FSM_state_t;

typedef enum {
	EVENT_TIMEOUT, EVENT_NO_ALARM, EVENT_ALARM
} FSM_event_t;

// Private global variable for the state
static FSM_state_t currentState;

/* Private function prototypes -----------------------------------------------*/

static void App_handleIdle(void);
static void App_handleReadSensor(void);
static void App_handleControl(void);
static void App_handleAlarm(void);

static FSM_event_t App_checkEvents(void);
static void App_updateDisplay(uint32_t temperature, uint32_t pressure, uint32_t humidity);
static void App_triggerAlarm(void);
static void App_resetAlarm(void);

/* Public Application code ---------------------------------------------------*/

/* Function Implementations --------------------------------------------------*/

void App_FSM_Init(void) {

}

void App_FSM_Update(void) {

	switch (currentState) {

	case STATE_IDLE:
		App_handleIdle();
		break;

	case STATE_READ_SENSOR:
		App_handleReadSensor();
		break;

	case STATE_CONTROL:
		App_handleControl();
		break;

	case STATE_ALARM:
		App_handleAlarm();
		break;

	default:
		currentState = STATE_IDLE;
		break;

	}

}

/* Private Application code ---------------------------------------------------*/

/* Function Implementations --------------------------------------------------*/

static void handleIdle(void) {

}

static void handleReadSensor(void) {

}

static void handleControl(void) {

}

static void handleAlarm(void) {

}

static FSM_event_t checkEvents(void) {

}

static void updateDisplay(uint32_t temperature, uint32_t pressure,
		uint32_t humidity) {

}

static void triggerAlarm(void) {

}

static void resetAlarm(void) {

}
