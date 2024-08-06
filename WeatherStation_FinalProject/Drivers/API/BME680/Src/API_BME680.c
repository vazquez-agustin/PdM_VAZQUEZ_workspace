/*
 * API_BME680.c
 *
 *  Created on: Aug 3, 2024
 *      Author: ubuntu
 */

/* Includes ------------------------------------------------------------------*/

#include "API_BME680.h"
#include "API_BME680_HAL.h"
#include "API_display.h"

/* Private define ------------------------------------------------------------*/



typedef struct {
    uint16_t par_t1;
    int16_t par_t2;
    int8_t par_t3;
} BME680_CalibData;

BME680_CalibData calib_data;

static void API_BME680_readRegister(uint8_t reg, uint8_t *pData, uint16_t size);

void API_BME680_readCalibrationData(void) {
    uint8_t buf[2];

    // Leer par_t1 (0xE9 / 0xEA)
    API_BME680_readRegister(0xE9, buf, 1);
    calib_data.par_t1 = buf[0];
    API_BME680_readRegister(0xEA, buf, 1);
    calib_data.par_t1 |= (uint16_t)buf[0] << 8;

    // Leer par_t2 (0x8A / 0x8B)
    API_BME680_readRegister(0x8A, buf, 1);
    calib_data.par_t2 = buf[0];
    API_BME680_readRegister(0x8B, buf, 1);
    calib_data.par_t2 |= (int16_t)buf[0] << 8;

    // Leer par_t3 (0x8C)
    API_BME680_readRegister(0x8C, buf, 1);
    calib_data.par_t3 = (int8_t)buf[0];

    // Convertir el valor de par_t3 a una cadena y mostrarlo en el display
    char buffer[16];  // Tamaño suficiente para contener el valor convertido
    snprintf(buffer, sizeof(buffer), "par_t3: %d", calib_data.par_t3);

    // Aquí puedes usar tus funciones de LCD para mostrar el valor
    API_display_SetCursor(0, 0);
    API_display_SendString(buffer);
}

static void API_BME680_readRegister(uint8_t reg, uint8_t *pData, uint16_t size) {

    reg |= 0x80;  // Set read bit

    API_BME680_selectPin(CS_Output_GPIO_Port, CS_Output_Pin);
    API_BME680_HAL_Transmit(pData, size);
    API_BME680_HAL_Receive(pData, size);
    API_BME680_deselectPin(CS_Output_GPIO_Port, CS_Output_Pin);
}


static uint8_t API_BME680_readChipID(void);

float API_BME680_readTemperature(void) {
    uint8_t temp_msb, temp_lsb, temp_xlsb;
    uint32_t temp_raw;

    API_BME680_readRegister(BME680_REG_TEMP_MSB, &temp_msb, 1);
    API_BME680_readRegister(BME680_REG_TEMP_LSB, &temp_lsb, 1);
    API_BME680_readRegister(BME680_REG_TEMP_XLSB, &temp_xlsb, 1);

    // Combinar los bytes leídos en un solo valor raw
    temp_raw = ((uint32_t)temp_msb << 12) | ((uint32_t)temp_lsb << 4) | ((uint32_t)(temp_xlsb >> 4));

    // Variables para los cálculos
    double var1, var2, t_fine;
    float temperature;

    // Cálculos para la compensación de temperatura usando punto flotante
    var1 = (((double)temp_raw / 16384.0) - ((double)calib_data.par_t1 / 1024.0)) * (double)calib_data.par_t2;
    var2 = ((((double)temp_raw / 131072.0) - ((double)calib_data.par_t1 / 8192.0)) *
            (((double)temp_raw / 131072.0) - ((double)calib_data.par_t1 / 8192.0))) *
           ((double)calib_data.par_t3 * 16.0);

    t_fine = var1 + var2;
    temperature = t_fine / 5120.0;

    return temperature;
}




/*

// Inicializar el BME680
void API_BME680_Init(void) {

	API_BME680_HAL_SPI_Init();
	API_BME680_HAL_Delay(1000);

	API_BME680_readChipID();
}

// Lee Chip ID
static uint8_t API_BME680_readChipID(void) {
	uint8_t reg = BME680_REG_CHIP_ID | 0x80;
	uint8_t chip_id = 0;

	API_BME680_selectPin(CS_Output_GPIO_Port, CS_Output_Pin);
	API_BME680_HAL_Transmit(&reg, sizeof(reg));
	API_BME680_HAL_Receive(&chip_id, sizeof(chip_id));
	API_BME680_deselectPin(CS_Output_GPIO_Port, CS_Output_Pin);

	return chip_id;

}









// Función para leer temperatura
void API_BME680_readTemperature(float *temperature) {
    uint8_t txData[3];
    uint8_t rxData[3];
    txData[0] = BME680_REG_TEMP_MSB; // Dirección del registro de temperatura

    API_BME680_HAL_Transmit(txData, sizeof(txData));
    API_BME680_HAL_Receive(rxData, sizeof(rxData));

    // Convertir los datos de temperatura (ajustar según el formato del sensor)
    int32_t temp_raw = (rxData[0] << 12) | (rxData[1] << 4) | (rxData[2] >> 4);
    *temperature = temp_raw / 100.0; // Ajusta la conversión según la especificación del sensor
}

// Función para leer humedad
void API_BME680_readHumidity(float *humidity) {
    uint8_t txData[2];
    uint8_t rxData[2];
    txData[0] = BME680_REG_HUM_MSB; // Dirección del registro de humedad

    API_BME680_HAL_Transmit(txData, sizeof(txData));
    API_BME680_HAL_Receive(rxData, sizeof(rxData));

    // Convertir los datos de humedad (ajustar según el formato del sensor)
    int16_t hum_raw = (rxData[0] << 8) | rxData[1];
    *humidity = hum_raw / 100.0; // Ajusta la conversión según la especificación del sensor
}

// Función para leer presión
void API_BME680_readPressure(float *pressure) {
    uint8_t txData[3];
    uint8_t rxData[3];
    txData[0] = BME680_REG_PRESS_MSB; // Dirección del registro de presión

    API_BME680_HAL_Transmit(txData, sizeof(txData));
    API_BME680_HAL_Receive(rxData, sizeof(rxData));

    // Convertir los datos de presión (ajustar según el formato del sensor)
    int32_t press_raw = (rxData[0] << 12) | (rxData[1] << 4) | (rxData[2] >> 4);
    *pressure = press_raw / 100.0; // Ajusta la conversión según la especificación del sensor
}

// Función para leer gases
void API_BME680_readGas(float *gas) {
    uint8_t txData[2];
    uint8_t rxData[2];
    txData[0] = BME680_REG_GAS_LSB; // Dirección del registro de gases

    API_BME680_HAL_Transmit(txData, sizeof(txData));
    API_BME680_HAL_Receive(rxData, sizeof(rxData));

    // Convertir los datos de gases (ajustar según el formato del sensor)
    int16_t gas_raw = (rxData[0] << 8) | rxData[1];
    *gas = gas_raw; // Ajusta la conversión según la especificación del sensor
}

*/
