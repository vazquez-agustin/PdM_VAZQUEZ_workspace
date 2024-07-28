
/* Includes ------------------------------------------------------------------*/
#include "API_uart.h"

/* Private variable ----------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;

/* Private function prototypes -----------------------------------------------*/
static void Error_Handler(void);


/*


uartSendString(uint8_t *pstring) recibe un puntero a un string que se desea enviar por la UART completo
(hasta el caracter ‘\0’) y debe utilizar la función de la HAL HAL_UART_Transmit(...) para transmitir el string.


uartSendStringSize(uint8_t * pstring, uint16_t size) recibe un puntero a un string que se desea enviar
por la UART y un entero con la cantidad de caracteres que debe enviar. La función debe utilizar
HAL_UART_Transmit(...) para transmitir el string.
 *
 * */
/* Public functions ----------------------------------------------------------*/

// Inicializa UART. Devuelve TRUE si es correcto, sino, FALSO
bool_t uartInit(void)
{
  UartHandle.Instance        = USARTx;

  UartHandle.Init.BaudRate   = 9600;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_ODD;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&UartHandle) != HAL_OK)

  {

    /* Initialization Error */
   Error_Handler();
   return false;

  }

  uartSendString((uint8_t*)"Welcome: UART initialized with 9600 baud rate, 1 stop bit.\r\n");
  return true;

}

void uartSendString(uint8_t * pstring) {

	if (pstring == NULL) return;

	uint16_t length = 0;
	while (pstring[length] != '\0') {

		length++;

	}

	HAL_UART_Transmit(UartHandle, pstring, length, HAL_MAX_DELAY);

}

void uartSendStringSize(uint8_t * pstring, uint16_t size) {

	if (pstring == NULL || size == 0 || size > 1024) { // Verificar límites razonables

		Error_Handler();

	}

	HAL_UART_Transmit(&UartHandle, pstring, size, HAL_MAX_DELAY);

}

void uartSendStringSize(uint8_t * pstring, uint16_t size) {

	if (pstring == NULL || size == 0 || size > 1024) { // Verificar límites razonables

		Error_Handler();

	}


	HAL_UART_Receive(&UartHandle, pstring, size, HAL_MAX_DELAY);

}

void uartReceiveStringSize(uint8_t * pstring, uint16_t size) {

	if (pstring == NULL || size == 0 || size > 1024) { // Verificar límites razonables

			Error_Handler();

		}

    HAL_UART_Receive(&UartHandle, pstring, size, HAL_MAX_DELAY);

}


