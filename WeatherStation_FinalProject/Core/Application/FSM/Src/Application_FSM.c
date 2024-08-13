/*
 * Application_FSM.c
 *
 *  Created on: Aug 8, 2024
 *      Author: Agustin Vazquez
 */

/* Includes ------------------------------------------------------------------*/

#include "Application_FSM.h"

/* Private types -------------------------------------------------------------*/

typedef enum {
	STATE_IDLE,
	STATE_READ_SENSOR,
	STATE_CONTROL,
	STATE_ALARM
} FSM_state_t;

typedef enum {
	NORMAL,
	ALARM
} FSM_statusAlarm_t;

static FSM_state_t currentState;
static delay_t delay;
static int32_t temperature;

/* Private define ------------------------------------------------------------*/

#define MAX_TEMP 85
#define MIN_TEMP -40
#define PERIOD 1000

/* Private function prototypes -----------------------------------------------*/

// Handles
static void App_handleIdle(void);
static void App_handleReadSensor(void);
static void App_handleControl(void);
static void App_handleAlarm(void);

// Activators
static void App_updateDisplay(FSM_statusAlarm_t status);
static void App_triggerAlarm(void);
static void App_resetAlarm(void);

// Alarm conditions
static bool_t App_checkAlarmCondition(void);
static bool_t App_checkAlarmResolved(void);

static void App_Error_Handler(void);

/* Public Application code ---------------------------------------------------*/

/* Function Implementations --------------------------------------------------*/

/**
  * @brief  Initializes the FSM and associated components.
  *         Sets the initial state to STATE_IDLE and initializes sensors, display, relay, and delay timer.
  * @retval None
  */
void App_FSM_Init(void) {

	currentState = STATE_IDLE;

	temperature = 0;

	API_BME680_Initialize();

	API_BME680_readCalibrationData();

	API_display_Init();

	API_Relay_Init();

	delayInit(&delay, PERIOD);

}

/**
  * @brief  Updates the FSM state based on the current state.
  *         Executes the corresponding state handler function and transitions the state accordingly.
  * @retval None
  */
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

/**
  * @brief  Handles the FSM in the idle state.
  *         Transitions to the STATE_READ_SENSOR state when the delay timer expires.
  * @retval None
  */
static void App_handleIdle(void) {

	if (delayRead(&delay)) {

		currentState = STATE_READ_SENSOR;

	}

}

/**
  * @brief  Handles the FSM in the sensor reading state.
  *         Reads the temperature sensor, calculates the temperature in Celsius,
  *         and transitions to the STATE_CONTROL state.
  * @retval None
  */
static void App_handleReadSensor(void) {

	// Read sensor measures
	uint32_t temp_adc = API_BME680_readTempADC();
	temperature = API_BME680_calculateTemperature(temp_adc);

	// Change to next state
	currentState = STATE_CONTROL;

}

/**
  * @brief  Handles the FSM in the control state.
  *         Checks for alarm conditions based on the temperature.
  *         Transitions to the STATE_ALARM state if an alarm condition is met,
  *         otherwise transitions back to STATE_IDLE.
  * @retval None
  */
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

/**
  * @brief  Handles the FSM in the alarm state.
  *         Activates the relay to trigger an alarm.
  *         Periodically checks if the alarm condition has been resolved.
  *         If resolved, deactivates the relay and transitions back to STATE_IDLE.
  * @retval None
  */
static void App_handleAlarm(void) {

	// Relay activated
	App_triggerAlarm();

	if (delayRead(&delay)) {

		// If TRUE, it comes back to STATE_IDLE

		if (App_checkAlarmResolved()) {

			App_resetAlarm();
			currentState = STATE_IDLE;

		}

	}

}

/**
  * @brief  Checks if the current temperature exceeds the alarm threshold.
  *         Verifies if the temperature is within a valid range and exceeds the defined threshold.
  *         If an alarm condition is met, returns true.
  * @retval bool_t: true if alarm condition is met, false otherwise.
  */
static bool_t App_checkAlarmCondition(void) {

	// Check temperature range of sensor
	if (temperature > MIN_TEMP && temperature < MAX_TEMP) {

		// Verify alarm conditions
		if (temperature > THRESHOLD_TEMP) {

			return true;

		}

		return false;

	}

	App_Error_Handler();

	return false;
}

/**
  * @brief  Checks if the alarm condition has been resolved.
  *         Re-reads the sensor and verifies if the temperature has returned to a normal level.
  *         If resolved, returns true.
  * @retval bool_t: true if alarm is resolved, false otherwise.
  */
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

/**
  * @brief  Updates the LCD display based on the FSM status.
  *         Displays the current temperature on the first line.
  *         If the FSM status is ALARM, displays an alarm message on the second line.
  *         Otherwise, clears the second line.
  * @param  FSM_statusAlarm_t status: current FSM status (NORMAL or ALARM)
  * @retval None
  */
static void App_updateDisplay(FSM_statusAlarm_t status) {

	char line1[17];
	sprintf(line1, "Temperature:%02ld C", temperature);

	API_display_setCursor(LINE1, 0);
	API_display_sendString(line1);

	API_display_setCursor(LINE2, 0);

	// If status is NORMAL, I want to check temperature normally
	if (status == NORMAL) {

		API_display_sendString("                 ");

	} else {

		// when status is ALARM, I want to display an alarm message
		API_display_sendString("      ALARMA     ");

	}

}

/**
  * @brief  Activates the relay to trigger an alarm condition.
  * @retval None
  */
static void App_triggerAlarm(void) {

	API_Relay_On();

}

/**
  * @brief  Deactivates the relay to reset the alarm condition.
  * @retval None
  */
static void App_resetAlarm(void) {

	API_Relay_Off();

}

/**
  * @brief  Handles critical errors by disabling interrupts and entering an infinite loop.
  *         Used as a last resort error handler when conditions are outside expected parameters.
  * @retval None
  */
static void App_Error_Handler(void) {

	__disable_irq();

	while (1) {

	}

}
