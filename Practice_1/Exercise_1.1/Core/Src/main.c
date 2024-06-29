/* Includes ------------------------------------------------------------------*/
#include "main.h" // Incluye el archivo de cabecera principal del proyecto

/* Function Prototypes -------------------------------------------------------*/
void SystemClock_Config(void);  // Configuración del reloj del sistema
static void MX_GPIO_Init(void); // Inicialización de los pines GPIO

/* Main Program --------------------------------------------------------------*/
int main(void)
{

  HAL_Init(); // Inicializa la biblioteca HAL (Hardware Abstraction Layer)
  SystemClock_Config(); // Configura el reloj del sistema
  MX_GPIO_Init(); // Inicializa los pines GPIO

  /* Infinite loop */
  while (1)
  {
    // Enciende LED durante 200ms
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // PIN_SET: Enciende LED1 (PB0)
    HAL_Delay(200); // Esperar 200 ms
    //Apaga todos los LEDs durante 200ms
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // PIN_RESET: Apaga LED1 (PB0)
    HAL_Delay(200); // Esperar 200 ms

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET); // PIN_SET: Enciende LED2 (PB7)
    HAL_Delay(200);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET); // PIN_RESET: Apaga LED2 (PB7)
    HAL_Delay(200);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); // PIN_SET: Enciende LED3 (PB14)
    HAL_Delay(200);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET); // PIN_RESET: Apaga LED3 (PB14)
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

/* Inicialización de los Pines GPIO ------------------------------------------*/
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0}; // Estructura para la configuración de GPIO

  __HAL_RCC_GPIOB_CLK_ENABLE(); // Habilita el reloj para el puerto GPIOB

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_7 | GPIO_PIN_14, GPIO_PIN_RESET); // Configura el nivel de los pines (bajo)

  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_7 | GPIO_PIN_14; // Configura los pines PB0, PB7 y PB14
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Configura los pines como salida push-pull
  /*
   *
   *
   * Un pin en "push-pull" tiene 2 estados:
   *
   *
   * Estado Alto (GPIO_PIN_SET): El pin se establece en un nivel de voltaje alto (generalmente igual
   * al voltaje de alimentación del microcontrolador).
   *
   *
   * Estado Bajo (GPIO_PIN_RESET): El pin se establece en un nivel de voltaje bajo (generalmente
   * cercano a 0 voltios).
   *
   *
   * */
  GPIO_InitStruct.Pull = GPIO_NOPULL; // Sin resistencias pull-up o pull-down
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Velocidad baja
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); // Inicializa los pines GPIOB con la configuración especificada
}

/* Manejo de Errores ---------------------------------------------------------*/
void Error_Handler(void)
{
  __disable_irq(); // Desactiva todas las interrupciones
  while (1)
  {
  }
}

