/*
 * API_BME680.h
 *
 *  Created on: Aug 3, 2024
 *      Author: ubuntu
 */

#ifndef API_BME680_INC_API_BME680_H_
#define API_BME680_INC_API_BME680_H_

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>
#include "API_BME680_Defs.h"

/* Function Prototypes -------------------------------------------------------*/

void API_BME680_Initialize(void);

void API_BME680_forceMeasurement(void);

//void API_BME680_writeRegister(uint8_t reg, uint8_t value);

//void API_BME680_readRegister(uint8_t reg, uint8_t* data);

uint8_t API_BME680_ConvertToHeaterRegister(int target_temp);

//void API_BME680_Init(void);
uint32_t API_BME680_readTempADC(void);
uint32_t API_BME680_readPressADC(void);
uint32_t API_BME680_readHumADC(void);
void API_BME680_setMemoryPage(uint8_t page);

int16_t API_BME680_calculateTemperature(uint32_t temp_adc);
double API_BME680_calculatePressure(uint32_t press_adc);

void API_BME680_readCalibrationData(void);

/*
float API_BME680_readPressure(void);
float API_BME680_readHumidity(float temp_comp);
uint8_t API_BME680_calculateHeaterResistance(uint16_t target_temp, int16_t amb_temp);
uint32_t API_BME680_readGasResistance(void);
*/

#endif /* API_BME680_INC_API_BME680_H_ */
