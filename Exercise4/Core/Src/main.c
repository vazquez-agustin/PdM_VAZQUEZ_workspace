
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"
#include "API_delay.h"
#include "API_debounce.h"

/* Private define ------------------------------------------------------------*/
#define LED_PIN GPIO_PIN_0
#define LED_GPIO_PORT GPIOB
static delay_t delay;
static delay_t delayLED;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);


int main(void)
{

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();

  // Inicializo la FSM
  debounceFSM_init();

  // Delay del antirebote
  delayInit(&delay, 40);

  // Frecuencias

  const uint32_t TIEMPOS[] = {100, 500};

  uint32_t index = 0;

  // Delay del LED2
  delayInit(&delayLED, TIEMPOS[0]);



  // Periféricos
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  BSP_LED_Init(LED1);

  BSP_LED_Init(LED2);

  while (1) {

	  if (delayRead(&delay)) {

		  // Lógica antirebote

		  if (BSP_PB_GetState(BUTTON_USER) == true) {

			  writeKey();

		  }

		  debounceFSM_update();

	  }

	  if (delayRead(&delay)) {

		  // Punto 4.1
		  if (isButtonDown()) {

			  BSP_LED_On(LED1);

		  } else {

			  BSP_LED_Off(LED1);

		  }


	   }

	  if (delayRead(&delayLED)) {

		  BSP_LED_Toggle(LED2);

	  }

	  if (isButtonDown()) {

		  delayWrite(&delayLED, TIEMPOS[index]);
		  index = (index + 1) % (sizeof(TIEMPOS) / sizeof(TIEMPOS[0]));

	  }


  }








	  /*
  delay_t ledDelay;
  uint8_t currentStep = 0;

  delayInit(&ledDelay, TIEMPOS[currentStep]);

  bool_t ledState = false;

  while (1)
  {
	  // Verifica si el retardo no bloqueante no está corriendo
	  if (!delayIsRunning(&ledDelay)) {

	      // Cambia el estado del LED: lo enciende si está apagado y lo apaga si está encendido
		  HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, ledState ? GPIO_PIN_SET : GPIO_PIN_RESET);
		  // Alterna el estado del LED
		  ledState = !ledState;

		  // Calcula el siguiente paso en la secuencia de tiempos
	      currentStep = (currentStep + 1) % (sizeof(TIEMPOS) / sizeof(TIEMPOS[0]));
	      // Actualiza la duración del retardo con el próximo tiempo en la secuencia
	      delayWrite(&ledDelay, TIEMPOS[currentStep]);

	      // Reinicia el retardo con la nueva duración
	      delayInit(&ledDelay, TIEMPOS[currentStep]);

      }

	  // Lee el estado del retardo para saber si ha expirado o no
	  delayRead(&ledDelay);

  }
  */

}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}



void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
