/*
 * API_BME680.h
 *
 *  Created on: Aug 3, 2024
 *      Author: ubuntu
 */

#ifndef API_BME680_INC_API_BME680_H_
#define API_BME680_INC_API_BME680_H_

typedef struct {
	float temperature;
	float humedity;
	float pressure;
	float gas;
} BME680_t;

#endif /* API_BME680_INC_API_BME680_H_ */
