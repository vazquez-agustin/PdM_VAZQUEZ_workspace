/*
 * API_BME680.h
 *
 *  Created on: Aug 3, 2024
 *      Author: ubuntu
 */

#ifndef API_BME680_INC_API_BME680_H_
#define API_BME680_INC_API_BME680_H_

#include <stdint.h>

typedef struct {
	float temperature;
	float humedity;
	float pressure;
	float gas;
} BME680_t;

// Inicializa el sensor BME680
int BME680_Init(void);

// Lee datos del sensor BME680
int BME680_ReadData(BME680_t *data);

// Configura el sensor BME680
int BME680_SetConfig(void);

// Calibra el sensor BME680
int BME680_Calibrate(void);

#endif /* API_BME680_INC_API_BME680_H_ */
