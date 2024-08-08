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

/* Private define ------------------------------------------------------------*/

// Memory page 0

// Chip ID
#define BME680_REG_CHIP_ID 0x50

// Memory page 1

// Temperature
#define BME680_REG_TEMP_MSB   0x22
#define BME680_REG_TEMP_LSB   0x23
#define BME680_REG_TEMP_XLSB  0x24
// Pressure
#define BME680_REG_PRESS_MSB  0x1F
#define BME680_REG_PRESS_LSB  0x20
#define BME680_REG_PRESS_XLSB 0x21
// Humidety
#define BME680_REG_HUM_MSB    0x25
#define BME680_REG_HUM_LSB    0x26
// Gas
#define BME680_REG_GAS_LSB    0x2B
#define BME680_REG_GAS_MSB    0x2A

/* Function Prototypes -------------------------------------------------------*/

void API_BME680_Init(void);

void API_BME680_readCalibrationData(void);
float API_BME680_readTemperature(void);
float API_BME680_readPressure(void);
float API_BME680_readHumidity(float temp_comp);
uint8_t API_BME680_calculateHeaterResistance(uint16_t target_temp, int16_t amb_temp);
uint32_t API_BME680_readGasResistance(void);


#endif /* API_BME680_INC_API_BME680_H_ */
