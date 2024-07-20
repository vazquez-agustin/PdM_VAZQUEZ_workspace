/* Includes ------------------------------------------------------------------*/
#include "main.h" // Incluye el archivo de cabecera principal del proyecto

/* Function Prototypes -------------------------------------------------------*/
void SystemClock_Config(void);  // Configuración del reloj del sistema

/* Main Program --------------------------------------------------------------*/
int main(void)
{

  HAL_Init(); // Inicializa la biblioteca HAL (Hardware Abstraction Layer)
  SystemClock_Config(); // Configura el reloj del sistema

  /* Initialize BSP Led for LED1, 2 & 3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /* Infinite loop */
  while (1)
  {

	BSP_LED_On(LED1);
	HAL_Delay(200);
	BSP_LED_Off(LED1);

	BSP_LED_On(LED2);
	HAL_Delay(200);
	BSP_LED_Off(LED2);

	BSP_LED_On(LED3);
	HAL_Delay(200);
	BSP_LED_Off(LED3);

	HAL_Delay(200);

  }
}

/* Configuración del Reloj del Sistema ---------------------------------------*/
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0}; // Estructura para la configuración de los osciladores
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0}; // Estructura para la configuración de los relojes

  __HAL_RCC_PWR_CLK_ENABLE(); // Habilita el reloj para el controlador de energía
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3); // Configura el regulador de voltaje

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI; // Usa el oscilador interno de alta velocidad (HSI)
  RCC_OscInitStruct.HSIState = RCC_HSI_ON; // Enciende el HSI
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT; // Usa el valor de calibración predeterminado
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE; // No usa el PLL
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler(); // Manejo de errores en la configuración del oscilador
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2; // Configura los tipos de reloj
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI; // Usa HSI como fuente de reloj del sistema
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1; // Configura el divisor del reloj AHB
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1; // Configura el divisor del reloj APB1
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1; // Configura el divisor del reloj APB2

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler(); // Manejo de errores en la configuración del reloj
  }
}



/* Manejo de Errores ---------------------------------------------------------*/
void Error_Handler(void)
{
  __disable_irq(); // Desactiva todas las interrupciones
  while (1)
  {
  }
}

