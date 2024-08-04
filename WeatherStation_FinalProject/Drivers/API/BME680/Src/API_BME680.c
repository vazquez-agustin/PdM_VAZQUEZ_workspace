/*
 * API_BME680.c
 *
 *  Created on: Aug 3, 2024
 *      Author: ubuntu
 */

#include "API_BME680.h"
#include "API_BME680_HAL.h"

/* Private define ------------------------------------------------------------*/

#define LOW 0
#define HIGH 1

/* Private function prototypes -----------------------------------------------*/

static void API_BME680_writeRegister(uint8_t reg, uint8_t value);
static void API_BME680_readRegister(uint8_t reg, uint8_t value);

// Inicializa el sensor BME680
int API_BME680_Init(void) {

}

// Lee datos del sensor BME680
int API_BME680_ReadData(BME680_t *data) {

}

// Configura el sensor BME680
int API_BME680_SetConfig(void) {

}

// Calibra el sensor BME680
int API_BME680_Calibrate(void) {

}

static void API_BME680_WriteRegister(uint8_t reg, uint8_t value) {
	uint8_t tx_buf[2] = { reg, value };
	return API_BME680_HAL_SPI_Write(tx_buf, tx_buf);
}

static void API_BME680_ReadRegister(uint8_t reg, uint8_t *value) {
	uint8_t tx_buf = reg;
	uint8_t rx_buf;

	if (API_BME680_HAL_SPI_Read(tx_buf, tx_buf)) {
		*value = rx_buf;
		return true;
	}

	return false;
}
