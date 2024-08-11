/*
 * API_Relay.c
 *
 *  Created on: Aug 8, 2024
 *      Author: ubuntu
 */

/* Includes ------------------------------------------------------------------*/

#include "API_Relay.h"
#include "API_Relay_HAL.h"

/* Public API code -----------------------------------------------------------*/

/* Function Implementations --------------------------------------------------*/

// Initialize relay module
void API_Relay_Init(void) {

	API_Relay_HAL_GPIO_Init();

}

// Activate the relay
void API_Relay_On(void) {

	API_Relay_HAL_deselectPin(Relay_Output_GPIO_Port, Relay_Output_Pin);

}

// Deactivate the relay
void API_Relay_Off(void) {

	API_Relay_HAL_selectPin(Relay_Output_GPIO_Port, Relay_Output_Pin);

}

// Alternate relay state
void API_Relay_Toggle(void) {

	API_Relay_HAL_Toggle(Relay_Output_GPIO_Port, Relay_Output_Pin);

}


