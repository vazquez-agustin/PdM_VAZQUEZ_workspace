/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "stm32f4xx_hal.h"

/* Private define ------------------------------------------------------------*/
#define LED1_PIN GPIO_PIN_0
#define LED2_PIN GPIO_PIN_7
#define LED3_PIN GPIO_PIN_14
#define LED_PORT GPIOB

#define BUTTON_PIN GPIO_PIN_13
#define BUTTON_PORT GPIOC

/* Private variables ---------------------------------------------------------*/
uint16_t led_pins_seq1[] = {LED1_PIN, LED2_PIN, LED3_PIN};
uint16_t led_pins_seq2[] = {LED1_PIN, LED3_PIN, LED2_PIN};

uint8_t num_leds = sizeof(led_pins_seq1) / sizeof(led_pins_seq1[0]);

uint8_t current_led = 0; // Índice del LED actual
uint8_t sequence = 0; // Secuencia actual: 0 - Secuencia alterna: 1

/* Function Prototypes -------------------------------------------------------*/
void SystemClock_Config(void);
void MX_GPIO_Init(void);
uint8_t ReadButton(void);

/* Main Program --------------------------------------------------------------*/
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  while (1)
  {
    if (ReadButton())
    {
      sequence = !sequence; // Alternar secuencia
      HAL_Delay(200); // Debounce delay
    }

    uint16_t *current_sequence;

    if (sequence == 0) {

    	current_sequence = led_pins_seq1;

	} else {

		current_sequence = led_pins_seq2;

	}

    HAL_GPIO_WritePin(LED_PORT, current_sequence[current_led], GPIO_PIN_SET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(LED_PORT, current_sequence[current_led], GPIO_PIN_RESET);
    HAL_Delay(200);

    current_led = (current_led + 1) % num_leds; // Mover al siguiente LED
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


/* Read Button with Debounce -------------------------------------------------*/

/*
 *
 * Esta función asegura que solo se reconozcan cambios de estado
 * del botón como pulsaciones válidas después de que haya pasado
 * un tiempo mínimo (debounceDelay) desde el último cambio detectado,
 * lo que ayuda a evitar lecturas erráticas debido a fluctuaciones
 * temporales en el estado del botón.
 *
 * */
uint8_t ReadButton(void)
{
  static uint8_t lastButtonState = GPIO_PIN_RESET;
  static uint32_t lastDebounceTime = 0;
  uint32_t debounceDelay = 50;

  uint8_t currentState = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);

  if (currentState != lastButtonState)
  {
    lastDebounceTime = HAL_GetTick();
  }

  if ((HAL_GetTick() - lastDebounceTime) > debounceDelay)
  {
    if (currentState == GPIO_PIN_SET)
    {
      lastButtonState = currentState;
      return 1;
    }
  }

  lastButtonState = currentState;
  return 0;
}

void Error_Handler(void)
{

  __disable_irq();
  while (1)
  {
  }
}
