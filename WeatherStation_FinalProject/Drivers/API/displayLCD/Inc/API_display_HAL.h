/*
 * API_display_HAL.h
 *
 *  Created on: Jul 30, 2024
 *      Author: ubuntu
 */

#ifndef API_DISPLAYLCD_INC_API_DISPLAY_HAL_H_
#define API_DISPLAYLCD_INC_API_DISPLAY_HAL_H_

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include "API_display.h"

/* HAL Includes --------------------------------------------------------------*/

#include "stm32f4xx_hal.h"

/* Function Prototypes -------------------------------------------------------*/


void API_display_HAL_Delay (uint32_t delay);

void API_display_HAL_I2C_Write (I2C_HandleTypeDef *hi2c, uint8_t address, uint8_t *measures, uint16_t size);

#endif /* API_DISPLAYLCD_INC_API_DISPLAY_HAL_H_ */
