
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "stm32f4xx_hal.h"

// Implementación de las funciones de retardo no bloqueante
void delayInit(delay_t *delay, tick_t duration) {
    delay->duration = duration;
    delay->running = false;
}

bool_t delayRead(delay_t *delay) {
    if (!delay->running) {
        delay->startTime = HAL_GetTick();
        delay->running = true;
        return false;
    } else {
        tick_t currentTime = HAL_GetTick();
        if (currentTime - delay->startTime >= delay->duration) {
            delay->running = false;
            return true;
        } else {
            return false;
        }
    }
}

void delayWrite(delay_t *delay, tick_t duration) {
    delay->duration = duration;
}


/* Private define ------------------------------------------------------------*/
#define LED_PIN GPIO_PIN_0
#define LED_GPIO_PORT GPIOB

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);


int main(void)
{

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();


  delay_t ledDelay;
  delayInit(&ledDelay, 100);

  // Estado del LED
  bool_t ledState = false;

  /* Infinite loop */
  while (1)
  {
	  // Verifica si el retardo ha terminado
	  if (delayRead(&ledDelay)) {
		  // Cambia el estado del LED
	      ledState = !ledState;
	      HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, ledState ? GPIO_PIN_SET : GPIO_PIN_RESET);

	      // Reinicia el retardo
	      delayInit(&ledDelay, 100);
      }
  }
  /* USER CODE END 3 */
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
