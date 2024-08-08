/*
 * API_BME680_HAL.h
 *
 *  Created on: Aug 3, 2024
 *      Author: ubuntu
 */

#ifndef API_BME680_INC_API_BME680_HAL_H_
#define API_BME680_INC_API_BME680_HAL_H_

/* HAL Includes --------------------------------------------------------------*/

#include "stm32f4xx_hal.h"

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* Private define ------------------------------------------------------------*/

#define CS_Output_Pin GPIO_PIN_15
#define CS_Output_GPIO_Port GPIOA

#define Relay_Output_Pin GPIO_PIN_4
#define Relay_Output_GPIO_Port GPIOE

/* Function Prototypes -------------------------------------------------------*/

void API_BME680_HAL_Delay(uint32_t delay);
void API_BME680_HAL_GPIO_Init(void);
void API_BME680_HAL_SPI_Init(void);
void API_BME680_HAL_Transmit(uint8_t *pData, uint16_t size);
void API_BME680_HAL_Receive(uint8_t *pData, uint16_t size);
void API_BME680_selectPin(uint8_t port, uint8_t pin);
void API_BME680_deselectPin(uint8_t port, uint8_t pin);

#endif /* API_BME680_INC_API_BME680_HAL_H_ */
