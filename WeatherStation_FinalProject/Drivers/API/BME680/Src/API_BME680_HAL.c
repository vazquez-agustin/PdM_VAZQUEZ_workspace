/*
 * API_BME680_HAL.c
 *
 *  Created on: Aug 3, 2024
 *      Author: ubuntu
 */

/* Includes ------------------------------------------------------------------*/

#include "API_display_HAL.h"

/* HAL API code --------------------------------------------------------------*/

// Inicializa la comunicación SPI con el sensor BME680
void BME680_HAL_Init(SPI_HandleTypeDef *hspi) {

}

// Enviar datos al BME680 a través de SPI
void BME680_HAL_Write(uint8_t reg, uint8_t *data, uint16_t size) {

}

// Recibir datos del BME680 a través de SPI
void BME680_HAL_Read(uint8_t reg, uint8_t *data, uint16_t size) {

}
