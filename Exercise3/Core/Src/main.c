
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "stm32f4xx_hal.h"
#include "API_delay.h"




/* Private define ------------------------------------------------------------*/
#define LED_PIN GPIO_PIN_0
#define LED_GPIO_PORT GPIOB

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

const uint32_t TIEMPOS[] = {500, 100, 100, 1000};


int main(void)
{

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();


  delay_t ledDelay;
  uint8_t currentStep = 0;

  delayInit(&ledDelay, TIEMPOS[currentStep]);

  bool_t ledState = false;

  /* Infinite loop */
  while (1)
  {
	  if (!delayIsRunning(&ledDelay)) {

		  HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, ledState ? GPIO_PIN_SET : GPIO_PIN_RESET);
	      ledState = !ledState;

	      currentStep = (currentStep + 1) % (sizeof(TIEMPOS) / sizeof(TIEMPOS[0]));
	      delayWrite(&ledDelay, TIEMPOS[currentStep]);

	      delayInit(&ledDelay, TIEMPOS[currentStep]);

      }

	      delayRead(&ledDelay);
  }

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
