/*
 * API_Relay_HAL.c
 *
 *  Created on: Aug 8, 2024
 *      Author: ubuntu
 */

/* Includes ------------------------------------------------------------------*/

#include "API_Relay_HAL.h"

/* HAL API code --------------------------------------------------------------*/

void API_Relay_HAL_GPIO_Init(void) {

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(Relay_Output_GPIO_Port, Relay_Output_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(CS_Output_GPIO_Port, CS_Output_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : Relay_Output_Pin */
	GPIO_InitStruct.Pin = Relay_Output_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(Relay_Output_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : CS_Output_Pin */
	GPIO_InitStruct.Pin = CS_Output_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(CS_Output_GPIO_Port, &GPIO_InitStruct);

}


void API_Relay_HAL_selectPin(uint8_t port, uint8_t pin) {

	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);

}

void API_Relay_HAL_deselectPin(uint8_t port, uint8_t pin) {

	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);

}

void API_Relay_HAL_Toggle(uint8_t port, uint8_t pin) {

	HAL_GPIO_TogglePin(port, pin);

}

