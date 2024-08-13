/*
 * Application_FSM.h
 *
 *  Created on: Aug 8, 2024
 *      Author: Agustin Vazquez
 */

#ifndef APPLICATION_FSM_INC_APPLICATION_FSM_H_
#define APPLICATION_FSM_INC_APPLICATION_FSM_H_

/* Includes ------------------------------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "API_BME680.h"
#include "API_Relay.h"
#include "API_display.h"
#include "API_delay.h"

/* HAL Includes --------------------------------------------------------------*/

#include "stm32f4xx_hal.h"

/* Exported types ------------------------------------------------------------*/

typedef bool bool_t;

/* Public define -------------------------------------------------------------*/

#define THRESHOLD_TEMP 24 // In °C
#define NORMAL_TEMP 22 // In °C

/* Exported functions prototypes ---------------------------------------------*/

void App_FSM_Init(void);
void App_FSM_Update(void);

#endif /* APPLICATION_FSM_INC_APPLICATION_FSM_H_ */
