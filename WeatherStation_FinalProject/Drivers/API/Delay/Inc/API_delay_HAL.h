/*
 * API_delay_HAL.h
 *
 *  Created on: Aug 13, 2024
 *      Author: Agustin Vazquez
 */

#ifndef API_DELAY_INC_API_DELAY_HAL_H_
#define API_DELAY_INC_API_DELAY_HAL_H_

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* HAL Includes --------------------------------------------------------------*/

#include "stm32f4xx_hal.h"

/* Function Prototypes -------------------------------------------------------*/

uint32_t API_delay_HAL_getTick(void);

#endif /* API_DELAY_INC_API_DELAY_HAL_H_ */
