/*
 * API_Relay_HAL.h
 *
 *  Created on: Aug 8, 2024
 *      Author: ubuntu
 */

#ifndef API_RELAY_INC_API_RELAY_HAL_H_
#define API_RELAY_INC_API_RELAY_HAL_H_

/* HAL Includes --------------------------------------------------------------*/

#include "stm32f4xx_hal.h"

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* Private define ------------------------------------------------------------*/

#define Relay_Output_Pin GPIO_PIN_4
#define Relay_Output_GPIO_Port GPIOE

#define CS_Output_Pin GPIO_PIN_15
#define CS_Output_GPIO_Port GPIOA

/* Function Prototypes -------------------------------------------------------*/

void API_Relay_HAL_GPIO_Init(void);
void API_Relay_HAL_selectPin(uint8_t port, uint8_t pin);
void API_Relay_HAL_deselectPin(uint8_t port, uint8_t pin);

#endif /* API_RELAY_INC_API_RELAY_HAL_H_ */
