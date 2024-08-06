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
/*
void API_BME680_readTemperature(float *temperature);
void API_BME680_readHumidity(float *humidity);
void API_BME680_readPressure(float *pressure);
void API_BME680_readGas(float *gas);
*/

void API_BME680_readCalibrationData(void);
float API_BME680_readTemperature(void);

/*
typedef struct {
	float temperature;
	float humedity;
	float pressure;
	float gas;
} BME680_t;

void BME680_Init(void);
void BME680_ReadReg(uint8_t reg, uint8_t *pData);
void BME680_WriteReg(uint8_t reg, uint8_t value);
*/

/*

// Lee datos del sensor BME680
void API_BME680_ReadData(BME680_t *data);

// Configura el sensor BME680
void API_BME680_SetConfig(void);

// Calibra el sensor BME680
void API_BME680_Calibrate(void);
*/
#endif /* API_BME680_INC_API_BME680_H_ */
