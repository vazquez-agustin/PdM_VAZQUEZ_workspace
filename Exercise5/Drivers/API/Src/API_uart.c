/*
 * API_uart.c
 *
 *  Created on: Aug 1, 2024
 *      Author: ubuntu
 */

/* Includes ------------------------------------------------------------------*/

#include "API_uart.h"
#include "stm32f4xx_hal.h"
#include <string.h>

void Error_Handler(void);

/* Private define ------------------------------------------------------------*/

#define TIMEOUT 100

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart6;

/* API code ------------------------------------------------------------*/

/**
 * @brief Inicializa el módulo UART con los parámetros especificados.
 *
 * Esta función configura el UART con la instancia USART6 y parámetros específicos
 * como baud rate, tamaño de palabra, bits de parada, paridad, modo de operación,
 * control de flujo de hardware y sobremuestreo. Si la inicialización es exitosa,
 * se envían los detalles de configuración a través de UART.
 *
 * @retval true si la inicialización es exitosa, false en caso contrario.
 */
bool_t uartInit() {
	huart6.Instance = USART6;
	huart6.Init.BaudRate = 115200;
	huart6.Init.WordLength = UART_WORDLENGTH_8B;
	huart6.Init.StopBits = UART_STOPBITS_1;
	huart6.Init.Parity = UART_PARITY_NONE;
	huart6.Init.Mode = UART_MODE_TX_RX;
	huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart6.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart6) != HAL_OK) {
		Error_Handler();
	}

	// TODO convert configurations to strings and avoid hard coded values
	uartSendString((uint8_t*) "UART configuration: \r\n");
	uartSendString((uint8_t*) "Instance: USART6\r");
	uartSendString((uint8_t*) "BaudRate: 115200\r\n");
	uartSendString((uint8_t*) "WordLength: UART_WORDLENGTH_8B\r\n");
	uartSendString((uint8_t*) "StopBits: UART_STOPBITS_1\r\n");
	uartSendString((uint8_t*) "Parity: UART_PARITY_NONE\r\n");
	uartSendString((uint8_t*) "Mode: UART_HWCONTROL_NONE\r\n");
	uartSendString((uint8_t*) "HwFlowCtl: UART_OVERSAMPLING_16\r\n");
	uartSendString((uint8_t*) "OverSampling: UART_OVERSAMPLING_16\r\n");
	uartSendString((uint8_t*) "Initialization successful \r\n");

	return true;
}

/**
 * @brief Envía una cadena de caracteres a través de UART.
 *
 * Esta función envía una cadena de caracteres completa a través del módulo UART.
 * Si el puntero de la cadena es NULL, se llama al manejador de errores.
 *
 * @param pstring Puntero a la cadena de caracteres a enviar.
 */
void uartSendString(uint8_t *pstring) {

	if (NULL == pstring) {
		Error_Handler();
	} else {
		HAL_UART_Transmit(&huart6, pstring, sizeof(pstring), TIMEOUT);
	}

}

/**
 * @brief Envía una cadena de caracteres parcial a través de UART.
 *
 * Esta función envía una cantidad específica de caracteres de una cadena a través del módulo UART.
 * Si el puntero de la cadena es NULL, se llama al manejador de errores.
 *
 * @param pstring Puntero a la cadena de caracteres a enviar.
 * @param size Cantidad de caracteres a enviar.
 */
void uartSendStringSize(uint8_t *pstring, uint16_t size) {

	if (NULL != pstring) {
		Error_Handler();
	} else {
		HAL_UART_Transmit(&huart6, pstring, size, TIMEOUT);
	}

}

/**
 * @brief Recibe una cadena de caracteres parcial a través de UART.
 *
 * Esta función recibe una cantidad específica de caracteres a través del módulo UART y los almacena
 * en el buffer proporcionado. Si el puntero del buffer es NULL, se llama al manejador de errores.
 *
 * @param pstring Puntero al buffer donde se almacenarán los caracteres recibidos.
 * @param size Cantidad de caracteres a recibir.
 */
void uartReceiveStringSize(uint8_t *pstring, uint16_t size) {

	if (NULL != pstring) {
		Error_Handler();
	} else {
		HAL_UART_Receive(&huart6, pstring, size, TIMEOUT);
	}

}
