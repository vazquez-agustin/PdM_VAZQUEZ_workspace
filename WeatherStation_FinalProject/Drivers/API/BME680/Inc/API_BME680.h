/*
 * API_BME680.h
 *
 *  Created on: Aug 3, 2024
 *      Author: Agustin Vazquez
 */

#ifndef API_BME680_INC_API_BME680_H_
#define API_BME680_INC_API_BME680_H_

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>
#include "API_BME680_Defs.h"

/* Function Prototypes -------------------------------------------------------*/

void API_BME680_setMemoryPage(uint8_t page);
void API_BME680_Initialize(void);

uint32_t API_BME680_readTempADC(void);
int16_t API_BME680_calculateTemperature(uint32_t temp_adc);

uint32_t API_BME680_readPressADC(void);
int16_t API_BME680_calculatePressure(uint32_t press_adc);

uint32_t API_BME680_readHumADC(void);
int16_t API_BME680_calculateHumidity(uint32_t hum_adc, uint32_t temp_comp);

void API_BME680_readCalibrationData(void);

#endif /* API_BME680_INC_API_BME680_H_ */
