/*
 * API_display_HAL.c
 *
 *  Created on: Jul 30, 2024
 *      Author: ubuntu
 */

/* Includes ------------------------------------------------------------------*/

#include "API_display_HAL.h"

/* Extern public variables ---------------------------------------------------*/

extern I2C_HandleTypeDef hi2c1;

/* Private define ------------------------------------------------------------*/

#define TIMEOUT HAL_MAX_DELAY

/* HAL API Code --------------------------------------------------------------*/

void API_display_HAL_Delay (uint32_t delay) {;

	/**
	  * @brief This function provides minimum delay (in milliseconds) based
	  *        on variable incremented.
	  * @note In the default implementation , SysTick timer is the source of time base.
	  *       It is used to generate interrupts at regular time intervals where uwTick
	  *       is incremented.
	  * @note This function is declared as __weak to be overwritten in case of other
	  *       implementations in user file.
	  * @param Delay specifies the delay time length, in milliseconds.
	  * @retval None
	  */
	HAL_Delay(delay);

}

void API_display_HAL_I2C_Write (I2C_HandleTypeDef *hi2c1, uint8_t address, uint8_t *measures) {  //REVISAR PARÁMETROS DE FUNCIÓN

	/**
	  * @brief  Transmits in master mode an amount of data in blocking mode.
	  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
	  *                the configuration information for the specified I2C.
	  * @param  DevAddress Target device address: The device 7 bits address value
	  *         in datasheet must be shifted to the left before calling the interface
	  * @param  pData Pointer to data buffer
	  * @param  Size Amount of data to be sent
	  * @param  Timeout Timeout duration
	  * @retval HAL status
	  */

	// Para enviar comandos y datos desde el STM32 (maestro) al LCD (esclavo).
	HAL_I2C_Master_Transmit(&hi2c1, address, &measures, sizeof(measures), TIMEOUT);

}
