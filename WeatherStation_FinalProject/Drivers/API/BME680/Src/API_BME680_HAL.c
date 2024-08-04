/*
 * API_BME680_HAL.c
 *
 *  Created on: Aug 3, 2024
 *      Author: ubuntu
 */

/* Includes ------------------------------------------------------------------*/

#include "API_BME680_HAL.h"

/* Extern public variables----------------------------------------------------*/

SPI_HandleTypeDef hspi1;

/* Private define ------------------------------------------------------------*/

#define TIMEOUT 100
#define CS_Pin GPIO_PIN_15
#define CS_Port GPIOA

/* HAL API code --------------------------------------------------------------*/

void API_BME680_HAL_Delay(uint32_t delay) {
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

/**
 * @brief SPI1 Initialization Function with BME680 sensor
 * @param None
 * @retval None
 */
void API_BME680_HAL_SPI_Init(void) {
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK) {
		Error_Handler();
	}

}

// Enviar datos al BME680 a través de SPI
void API_BME680_HAL_SPI_Write(uint8_t *data, uint16_t size) {
	/**
	 * @brief  Transmit an amount of data in blocking mode.
	 * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
	 *               the configuration information for SPI module.
	 * @param  pData pointer to data buffer
	 * @param  Size amount of data to be sent
	 * @param  Timeout Timeout duration
	 * @retval HAL status
	 */
	HAL_SPI_Transmit(&hspi1, data, size, TIMEOUT);
}

// Recibir datos del BME680 a través de SPI
void API_BME680_HAL_SPI_Read(uint8_t *data, uint16_t size) {
	/**
	 * @brief  Receive an amount of data in blocking mode.
	 * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
	 *               the configuration information for SPI module.
	 * @param  pData pointer to data buffer
	 * @param  Size amount of data to be received
	 * @param  Timeout Timeout duration
	 * @retval HAL status
	 */
	HAL_SPI_Receive(&hspi1, data, size, TIMEOUT);
}

void API_BME680_HAL_SPI_WritePin(uint8_t state) {
	/**
	 * @brief  Sets or clears the selected data port bit.
	 *
	 * @note   This function uses GPIOx_BSRR register to allow atomic read/modify
	 *         accesses. In this way, there is no risk of an IRQ occurring between
	 *         the read and the modify access.
	 *
	 * @param  GPIOx where x can be (A..K) to select the GPIO peripheral for STM32F429X device or
	 *                      x can be (A..I) to select the GPIO peripheral for STM32F40XX and STM32F427X devices.
	 * @param  GPIO_Pin specifies the port bit to be written.
	 *          This parameter can be one of GPIO_PIN_x where x can be (0..15).
	 * @param  PinState specifies the value to be written to the selected bit.
	 *          This parameter can be one of the GPIO_PinState enum values:
	 *            @arg GPIO_PIN_RESET: to clear the port pin
	 *            @arg GPIO_PIN_SET: to set the port pin
	 * @retval None
	 */
	HAL_GPIO_WritePin(CS_Port, CS_Pin, state);
}
