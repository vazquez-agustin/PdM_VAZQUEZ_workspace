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

#define TIMEOUT 1000

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
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
void API_BME680_HAL_GPIO_Init(void) {

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(CS_Output_GPIO_Port, CS_Output_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : CS_Output_Pin */
	GPIO_InitStruct.Pin = CS_Output_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(CS_Output_GPIO_Port, &GPIO_InitStruct);

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

// Transmitir datos a través de SPI
void API_BME680_HAL_Transmit(uint8_t *pData, uint16_t size) {

	HAL_SPI_Transmit(&hspi1, pData, size, TIMEOUT);

}

// Recibir datos a través de SPI
void API_BME680_HAL_Receive(uint8_t *pData, uint16_t size) {

	HAL_SPI_Receive(&hspi1, pData, size, TIMEOUT);

}

// Control del pin CS (Chip Select)
void API_BME680_selectPin(void) {

	HAL_GPIO_WritePin(CS_Output_GPIO_Port, CS_Output_Pin, GPIO_PIN_RESET);

}

void API_BME680_deselectPin(void) {

	HAL_GPIO_WritePin(CS_Output_GPIO_Port, CS_Output_Pin, GPIO_PIN_SET);

}
