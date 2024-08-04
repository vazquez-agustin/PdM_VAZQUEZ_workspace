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

void API_BME680_HAL_Delay(uint32_t delay);

// Inicializa la comunicación SPI con el sensor BME680
void API_BME680_HAL_SPI_Init(void);

// Enviar datos al BME680 a través de SPI
void API_BME680_HAL_SPI_Write(uint8_t *data, uint16_t size);

// Recibir datos del BME680 a través de SPI
void API_BME680_HAL_SPI_Read(uint8_t *data, uint16_t size);

// Setea pines de GPIO en HIGH o LOW
void API_BME680_HAL_SPI_WritePin(uint8_t state);

#endif /* API_BME680_INC_API_BME680_HAL_H_ */
