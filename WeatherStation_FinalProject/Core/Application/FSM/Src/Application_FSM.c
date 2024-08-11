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
	NORMAL, ALARM
} FSM_statusAlarm_t;

// Private global variable for the state
static FSM_state_t currentState;
static delay_t delay;
static int32_t temperature;

#define MAX_TEMP 85
#define MIN_TEMP -40
#define PERIOD 1000

/* Private function prototypes -----------------------------------------------*/

static void App_handleIdle(void);
static void App_handleReadSensor(void);
static void App_handleControl(void);
static void App_handleAlarm(void);

static void App_updateDisplay(FSM_statusAlarm_t status);
static void App_triggerAlarm(void);
static void App_resetAlarm(void);
static bool_t App_checkAlarmCondition(void);
static bool_t App_checkAlarmResolved(void);

static void App_Error_Handler(void);

/* Public Application code ---------------------------------------------------*/

/* Function Implementations --------------------------------------------------*/

void App_FSM_Init(void) {

	currentState = STATE_IDLE;

	temperature = 0;

	API_BME680_Initialize();

	API_BME680_readCalibrationData();

	API_display_Init();

	API_Relay_Init();

	delayInit(&delay, PERIOD);

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

static void App_handleIdle(void) {

	if (delayRead(&delay)) {

		currentState = STATE_READ_SENSOR;

	}

}

static void App_handleReadSensor(void) {

	// Read sensor measures
	uint32_t temp_adc = API_BME680_readTempADC();
	temperature = API_BME680_calculateTemperature(temp_adc);

	// Change to next state
	currentState = STATE_CONTROL;

}

static void App_handleControl(void) {

	bool_t alarmCondition = App_checkAlarmCondition();

	if (alarmCondition) {

		currentState = STATE_ALARM;
		App_updateDisplay(ALARM);

	} else {

		currentState = STATE_IDLE;
		App_updateDisplay(NORMAL);

	}

}

static void App_handleAlarm(void) {

	// Relay activated
	App_triggerAlarm();

	if (delayRead(&delay)) {

		// Verificar condiciones para volver al estado IDLE

		if (App_checkAlarmResolved()) {

			App_resetAlarm();
			currentState = STATE_IDLE;

		}

	}

}

static bool_t App_checkAlarmCondition(void) {

	if (temperature > MIN_TEMP && temperature < MAX_TEMP) {

		// Verify alarm contiditions
		if (temperature > THRESHOLD_TEMP) {

			return true;

		}

		return false;

	}

	App_Error_Handler();
	return false;
}

static bool_t App_checkAlarmResolved(void) {

	// Read sensor measures
	uint32_t temp_adc = API_BME680_readTempADC();
	temperature = API_BME680_calculateTemperature(temp_adc);

	if (temperature > MIN_TEMP && temperature < MAX_TEMP) {

		// Verify alarm contiditions
		if (temperature <= NORMAL_TEMP) {

			return true;

		}

		return false;

	}

	App_Error_Handler();
	return false;
}

static void App_updateDisplay(FSM_statusAlarm_t status) {

	//API_display_Clear();

	char line1[17];
	sprintf(line1, "Temp:%02ldC", temperature);

	API_display_SetCursor(LINE1, 0);
	API_display_SendString(line1);

	API_display_SetCursor(LINE2, 0);

	if (status == NORMAL) {

		API_display_SendString("                 ");

	} else {

		API_display_SendString("      ALARMA     ");

	}

}

static void App_triggerAlarm(void) {

	API_Relay_On();

}

static void App_resetAlarm(void) {

	API_Relay_Off();

}

static void App_Error_Handler(void) {

	__disable_irq();

	while (1) {

	}

}
