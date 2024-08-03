/*
 * API_BME680_HAL.h
 *
 *  Created on: Aug 3, 2024
 *      Author: ubuntu
 */

#ifndef API_BME680_INC_API_BME680_HAL_H_
#define API_BME680_INC_API_BME680_HAL_H_

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* HAL Includes --------------------------------------------------------------*/

#include "stm32f4xx_hal.h"

/* Function Prototypes -------------------------------------------------------*/

// Inicializa la comunicación SPI con el sensor BME680
void BME680_HAL_Init(SPI_HandleTypeDef *hspi);

// Enviar datos al BME680 a través de SPI
void BME680_HAL_Write(uint8_t reg, uint8_t *data, uint16_t size);

// Recibir datos del BME680 a través de SPI
void BME680_HAL_Read(uint8_t reg, uint8_t *data, uint16_t size);

#endif /* API_BME680_INC_API_BME680_HAL_H_ */
