
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "stm32f4xx_hal.h"
#include "API_delay.h"
//#include "API_debounce.h"

/* Private define ------------------------------------------------------------*/
#define LED_PIN GPIO_PIN_0
#define LED_GPIO_PORT GPIOB

// Variables de tiempo
const uint32_t TIEMPO1 = 500;
const uint32_t TIEMPO2 = 100;
const uint32_t TIEMPO3 = 100;
const uint32_t TIEMPO4 = 1000;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

const uint32_t TIEMPOS[] = {TIEMPO1, TIEMPO2, TIEMPO3, TIEMPO4};


int main(void)
{

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();


  //debounceFSM_init();


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
