/*
 * API_Relay.c
 *
 *  Created on: Aug 8, 2024
 *      Author: Agustin Vazquez
 */

/* Includes ------------------------------------------------------------------*/

#include "API_Relay.h"
#include "API_Relay_HAL.h"

/* Public API code -----------------------------------------------------------*/

/* Function Implementations --------------------------------------------------*/

/**
  * @brief  Initialize the relay module.
  * @retval None
  */
void API_Relay_Init(void) {

	API_Relay_HAL_GPIO_Init();

}

/**
  * @brief  Activate the relay.
  * @retval None
  */
void API_Relay_On(void) {

	API_Relay_HAL_deselectPin(Relay_Output_GPIO_Port, Relay_Output_Pin);

}

/**
  * @brief  Deactivate the relay.
  * @retval None
  */
void API_Relay_Off(void) {

	API_Relay_HAL_selectPin(Relay_Output_GPIO_Port, Relay_Output_Pin);

}
