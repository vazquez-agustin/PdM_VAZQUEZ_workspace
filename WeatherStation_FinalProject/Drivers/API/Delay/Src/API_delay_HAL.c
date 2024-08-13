/*
 * API_delay_HAL.c
 *
 *  Created on: Aug 13, 2024
 *      Author: Agustin Vazquez
 */

/* Includes ------------------------------------------------------------------*/

#include "API_delay_HAL.h"

/* HAL API code --------------------------------------------------------------*/

/**
 * @brief  Provides the current tick value in milliseconds.
 * @retval Current tick value.
 */
uint32_t API_delay_HAL_getTick(void) {

	/**
	 * @brief Provides a tick value in millisecond.
	 * @note This function is declared as __weak to be overwritten in case of other
	 *       implementations in user file.
	 * @retval tick value
	 */
	return HAL_GetTick(); // Return the current tick value.

}
