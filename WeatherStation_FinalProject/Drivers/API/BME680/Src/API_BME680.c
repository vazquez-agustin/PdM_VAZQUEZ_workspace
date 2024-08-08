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

float temp_comp;

typedef struct {

	// temperature
	uint16_t par_t1;
	int16_t par_t2;
	int8_t par_t3;
	int16_t temp_adc;
	// pressure
	uint16_t par_p1;
	int16_t par_p2;
	int8_t par_p3;
	int16_t par_p4;
	int16_t par_p5;
	int8_t par_p6;
	int8_t par_p7;
	int16_t par_p8;
	int16_t par_p9;
	uint8_t par_p10;
	int32_t press_adc;
	// humidity
    uint16_t par_h1;
    uint16_t par_h2;
    int8_t par_h3;
    int8_t par_h4;
    int8_t par_h5;
    uint8_t par_h6;
    int8_t par_h7;
    int32_t hum_adc;
    // gas
    uint8_t par_g1;
    int16_t par_g2;
    int8_t par_g3;
    uint8_t res_heat_range;
    int8_t res_heat_val;
    int32_t gas_adc;
    uint8_t gas_range;
    int8_t range_switching_error;

} BME680_CalibData;

BME680_CalibData calib_data;

static void API_BME680_readRegister(uint8_t reg, uint8_t *pData, uint16_t size);

static void API_BME680_readCalibrationTemperature(void);
static void API_BME680_readCalibrationPressure(void);
static void API_BME680_readCalibrationHumidity(void);
static void API_BME680_readCalibrationGas(void);

static uint8_t API_BME680_readChipID(void);

/* Public API code -----------------------------------------------------------*/

/* Function Implementations --------------------------------------------------*/

// Initialize BME680 by reading ChipID
void API_BME680_Init(void) {

	API_BME680_HAL_SPI_Init();
	API_BME680_HAL_Delay(1000);

	API_BME680_readChipID();

}

// Read Temperature
float API_BME680_readTemperature(void) {

	//uint8_t temp_msb, temp_lsb, temp_xlsb;
	//uint32_t temp_raw;
	// Variables para los cálculos
	float var1, var2, t_fine;

	uint8_t temp_msb = API_BME680_readRegister(BME680_REG_TEMP_MSB, &temp_msb, 1);
	uint8_t temp_lsb = API_BME680_readRegister(BME680_REG_TEMP_LSB, &temp_lsb, 1);
	uint8_t temp_xlsb = API_BME680_readRegister(BME680_REG_TEMP_XLSB, &temp_xlsb, 1);

	// Combinar los bytes leídos en un solo valor raw
	uint32_t temp_raw = ((uint32_t) temp_msb << 12) | ((uint32_t) temp_lsb << 4) | ((uint32_t) (temp_xlsb >> 4));

	//float temperature;

	// Calculate var1 data
	var1 = (((float) temp_raw / 16384.0) - ((float) calib_data.par_t1 / 1024.0)) * (float) calib_data.par_t2;

	// Calculate var2 data
	var2 = ((((float) temp_raw / 131072.0) - ((float) calib_data.par_t1 / 8192.0)) *
			 (((float) temp_raw / 131072.0) - ((float) calib_data.par_t1 / 8192.0))) *
			 ((float) calib_data.par_t3 * 16.0);

	// t_fine value
	t_fine = var1 + var2;

	temp_comp = t_fine / 5120.0;

	return temp_comp;

}

// Read Pressure
float API_BME680_readPressure(void) {

	double var1, var2, var3, press_comp;

	uint8_t press_msb = API_BME680_readRegister(BME680_REG_PRESS_MSB, &press_msb, 1);
	uint8_t press_lsb = API_BME680_readRegister(BME680_REG_PRESS_LSB, &press_lsb, 1);
	uint8_t press_xlsb = API_BME680_readRegister(BME680_REG_PRESS_XLSB, &press_xlsb, 1);
	int32_t press_raw = ((uint32_t) press_msb << 12) | ((uint32_t) press_lsb << 4) | ((uint32_t) press_xlsb >> 4);

	var1 = ((float)t_fine / 2.0) - 64000.0;
	var2 = var1 * var1 * ((float)calib_data.par_p6 / 131072.0);
	var2 = var2 + (var1 * (float)calib_data.par_p5 * 2.0);
	var2 = (var2 / 4.0) + ((float)calib_data.par_p4 * 65536.0);
	var1 = ((((float)calib_data.par_p3 * var1 * var1) / 16384.0) +
			 ((float)calib_data.par_p2 * var1)) / 524288.0;
	var1 = (1.0 + (var1 / 32768.0)) * (float)calib_data.par_p1;
	press_comp = 1048576.0 - (float)calib_data.press_adc;

	// Avoid division by 0 if var1 = 0
	if ((int)var1 != 0) {

		press_comp = ((press_comp - (var2 / 4096.0)) * 6250.0) / var1;
		var1 = ((float)calib_data.par_p9 * press_comp * press_comp) / 2147483648.0;
		var2 = press_comp * ((float)calib_data.par_p8 / 32768.0);
		var3 = (press_comp / 256.0) * (press_comp / 256.0) *
			   (press_comp / 256.0) * (calib_data.par_p10 / 131072.0);
		press_comp = press_comp + (var1 + var2 + var3 +	((float)calib_data.par_p7 * 128.0)) / 16.0;

	} else {

		press_comp = 0;

	}

	return press_comp;

}

// Read Humidity
float API_BME680_readHumidity(float temp_comp) {

	float var1, var2, var3, var4, hum_comp;

	uint8_t hum_msb = API_BME680_readRegister(BME680_REG_HUM_MSB, &hum_msb, 1);
	uint8_t hum_lsb = API_BME680_readRegister(BME680_REG_HUM_LSB, &hum_lsb, 1);
	int32_t hum_raw = ((uint32_t) hum_msb << 8) | (uint32_t) hum_lsb;

	// Calculate var1 data
	var1 = calib_data.hum_adc - (((float)calib_data.par_h1 * 16.0) + (((float)calib_data.par_h3 / 2.0) * temp_comp));

	// Calculate var2 data
	var2 = var1 * (((float)calib_data.par_h2 / 262144.0) * (1.0 + (((float)calib_data.par_h4 / 16384.0) * temp_comp) +
				  (((float)calib_data.par_h5 / 1048576.0) * temp_comp * temp_comp)));

	// Calculate var3 data
	var3 = (float)calib_data.par_h6 / 16384.0;

	// Calculate var4 data
	var4 = (float)calib_data.par_h7 / 2097152.0;


	hum_comp = var2 + ((var3 + (var4 * temp_comp)) * var2 * var2);

	// Compensación de humedad de acuerdo a la fórmula proporcionada
	var1 = hum_raw - (((float)calib_data.par_h1 * 16.0) + (((float)calib_data.par_h3 / 2.0) * temp_comp));
	var2 = var1 * (((float)calib_data.par_h2 / 262144.0) * (1.0 + (((float)calib_data.par_h4 / 16384.0) * temp_comp) +
			      (((float)par_h5 / 1048576.0) * temp_comp * temp_comp)));
	var3 = (float)calib_data.par_h6 / 16384.0;
	var4 = (float)calib_data.par_h7 / 2097152.0;
	hum_comp = var2 + ((var3 + (var4 * temp_comp)) * var2 * var2);

	return hum_comp;

}

// Calculate Resistance of heater
uint8_t API_BME680_calculateHeaterResistance(uint16_t target_temp, int16_t amb_temp) {

    double var1, var2, var3, var4, var5;
    uint8_t res_heat_x;

    var1 = ((double)calib_data.par_g1 / 16.0) + 49.0;
    var2 = (((double)calib_data.par_g2 / 32768.0) * 0.0005) + 0.00235;
    var3 = (double)calib_data.par_g3 / 1024.0;
    var4 = var1 * (1.0 + (var2 * (double)target_temp));
    var5 = var4 + (var3 * (double)amb_temp);
    res_heat_x = (uint8_t)(3.4 * ((var5 * (4.0 / (4.0 + (double)calib_data.res_heat_range)) *
                (1.0 / (1.0 + ((double)calib_data.res_heat_val * 0.002)))) - 25));

    return res_heat_x;

}

// Read Gas resistance
uint32_t API_BME680_readGasResistance(void) {

    uint8_t buf[2];

    // Leer gas_adc (0x2B<7:6> / 0x2A)
    uint8_t gas_adc_msb = API_BME680_readRegister(BME680_REG_GAS_MSB, &gas_adc_msb, 1);
    uint8_t gas_adc_lsb = API_BME680_readRegister(BME680_REG_GAS_LSB, &gas_adc_lsb, 1);


    calib_data.gas_adc = (gas_adc_msb << 2) | (gas_adc_lsb >> 6);

    // Leer gas_range (0x2B<3:0>)
    calib_data.gas_range = gas_adc_lsb & 0x0F;

    // Leer range_switching_error (0x04 <7:4>)
    API_BME680_readRegister(0x04, buf, 1);
    calib_data.range_switching_error = (int8_t)(buf[0] >> 4);

    // Calcular la resistencia del gas
    double var1 = (1340.0 + 5.0 * calib_data.range_switching_error) * const_array1[calib_data.gas_range];
    double gas_res = var1 * const_array2[calib_data.gas_range] / (calib_data.gas_adc - 512.0 + var1);

    return (uint32_t)gas_res;

}

/* Private API code ----------------------------------------------------------*/

/* Function Implementations --------------------------------------------------*/

static void API_BME680_readRegister(uint8_t reg, uint8_t *pData, uint16_t size) {

	reg |= 0x80;  // Set read bit

	API_BME680_selectPin(CS_Output_GPIO_Port, CS_Output_Pin);
	API_BME680_HAL_Transmit(pData, size);
	API_BME680_HAL_Receive(pData, size);
	API_BME680_deselectPin(CS_Output_GPIO_Port, CS_Output_Pin);

}

// Calibration of Temperature
static void API_BME680_readCalibrationTemperature(void) {

	uint8_t buf[2];

	// Read par_t1 (0xE9 / 0xEA)
	API_BME680_readRegister(0xE9, buf, 1);
	calib_data.par_t1 = buf[0];
	API_BME680_readRegister(0xEA, buf, 1);
	calib_data.par_t1 |= (uint16_t) buf[0] << 8;

	// Read par_t2 (0x8A / 0x8B)
	API_BME680_readRegister(0x8A, buf, 1);
	calib_data.par_t2 = buf[0];
	API_BME680_readRegister(0x8B, buf, 1);
	calib_data.par_t2 |= (int16_t) buf[0] << 8;

	// Read par_t3 (0x8C)
	API_BME680_readRegister(0x8C, buf, 1);
	calib_data.par_t3 = (int8_t) buf[0];

	// Leer temp_adc (0x24<7:4> / 0x23 / 0x22)
	uint8_t temp_adc_msb; // MSB
	uint8_t temp_adc_mid; // Middle byte
	uint8_t temp_adc_lsb; // LSB

	// Read registers
	API_BME680_readRegister(0x24, &temp_adc_msb, 1); // Lee los bits 7:4 del MSB
	API_BME680_readRegister(0x23, &temp_adc_mid, 1); // Lee el byte medio
	API_BME680_readRegister(0x22, &temp_adc_lsb, 1); // Lee el LSB

	// Ensamblar el valor completo de temp_adc
	calib_data.temp_adc = (temp_adc_msb & 0xF0) << 12 | // Bits 7-4 del MSB
			(temp_adc_mid << 4) | // Byte medio
			(temp_adc_lsb >> 4); // Bits 3-0 del LSB

}

// Calibration of Pressure
static void API_BME680_readCalibrationPressure(void) {

	uint8_t buf[2];

	// Leer par_p1 (0x8E / 0x8F)
	API_BME680_readRegister(0x8E, buf, 1);
	calib_data.par_p1 = buf[0];
	API_BME680_readRegister(0x8F, buf, 1);
	calib_data.par_p1 |= (uint16_t)buf[0] << 8;

	// Leer par_p2 (0x90 / 0x91)
	API_BME680_readRegister(0x90, buf, 1);
	calib_data.par_p2 = buf[0];
	API_BME680_readRegister(0x91, buf, 1);
	calib_data.par_p2 |= (int16_t)buf[0] << 8;

	// Leer par_p3 (0x92)
    API_BME680_readRegister(0x92, buf, 1);
	calib_data.par_p3 = (int8_t)buf[0];

	    // Leer par_p4 (0x94 / 0x95)
    API_BME680_readRegister(0x94, buf, 1);
    calib_data.par_p4 = buf[0];
    API_BME680_readRegister(0x95, buf, 1);
    calib_data.par_p4 |= (int16_t)buf[0] << 8;

    // Leer par_p5 (0x96 / 0x97)
	API_BME680_readRegister(0x96, buf, 1);
    calib_data.par_p5 = buf[0];
    API_BME680_readRegister(0x97, buf, 1);
    calib_data.par_p5 |= (int16_t)buf[0] << 8;

    // Leer par_p6 (0x99)
    API_BME680_readRegister(0x99, buf, 1);
    calib_data.par_p6 = (int8_t)buf[0];

    // Leer par_p7 (0x98)
    API_BME680_readRegister(0x98, buf, 1);
    calib_data.par_p7 = (int8_t)buf[0];

    // Leer par_p8 (0x9C / 0x9D)
    API_BME680_readRegister(0x9C, buf, 1);
    calib_data.par_p8 = buf[0];
    API_BME680_readRegister(0x9D, buf, 1);
    calib_data.par_p8 |= (int16_t)buf[0] << 8;

    // Leer par_p9 (0x9E / 0x9F)
    API_BME680_readRegister(0x9E, buf, 1);
    calib_data.par_p9 = buf[0];
    API_BME680_readRegister(0x9F, buf, 1);
    calib_data.par_p9 |= (int16_t)buf[0] << 8;

    // Leer par_p10 (0xA0)
    API_BME680_readRegister(0xA0, buf, 1);
    calib_data.par_p10 = buf[0];

    // Leer press_adc (0x21<7:4> / 0x20 / 0x1F)
    uint8_t press_adc_msb; // MSB
    uint8_t press_adc_mid; // Middle byte
    uint8_t press_adc_lsb; // LSB

    // Read registers
    API_BME680_readRegister(0x21, &press_adc_msb, 1); // Lee los bits 7:4 del MSB
    API_BME680_readRegister(0x20, &press_adc_mid, 1); // Lee el byte medio
    API_BME680_readRegister(0x1F, &press_adc_lsb, 1); // Lee el LSB

    // Ensamblar el valor completo de press_adc
    calib_data.press_adc = (press_adc_msb & 0xF0) << 12 | // Bits 7-4 del MSB
                           (press_adc_mid << 4) | // Byte medio
	                       (press_adc_lsb >> 4); // Bits 3-0 del LSB

}

// Calibration of Humidity
static void API_BME680_readCalibrationHumidity(void) {

    uint8_t buf[2];

    // Leer par_h1 (0xE2<7:4>, 0xE3)
    API_BME680_readRegister(0xE2, buf, 1);
    uint8_t e2_lsb = buf[0] & 0x0F; // Bits 3-0 del LSB de par_h1
    API_BME680_readRegister(0xE3, buf, 1);
    calib_data.par_h1 = ((uint16_t)buf[0] << 4) | e2_lsb;

    // Leer par_h2 (0xE1 / 0xE2<3:0>)
    API_BME680_readRegister(0xE1, buf, 1);
    uint8_t e2_msb = buf[0];
    calib_data.par_h2 = ((uint16_t)e2_msb << 4) | ((buf[0] & 0xF0) >> 4);

    // Leer par_h3 (0xE4)
    API_BME680_readRegister(0xE4, buf, 1);
    calib_data.par_h3 = (int8_t)buf[0];

    // Leer par_h4 (0xE5)
    API_BME680_readRegister(0xE5, buf, 1);
    calib_data.par_h4 = (int8_t)buf[0];

    // Leer par_h5 (0xE6)
    API_BME680_readRegister(0xE6, buf, 1);
    calib_data.par_h5 = (int8_t)buf[0];

    // Leer par_h6 (0xE7)
    API_BME680_readRegister(0xE7, buf, 1);
    calib_data.par_h6 = buf[0];

    // Leer par_h7 (0xE8)
    API_BME680_readRegister(0xE8, buf, 1);
    calib_data.par_h7 = (int8_t)buf[0];

    // Leer hum_adc (0x26 / 0x25)
    uint8_t hum_adc_msb; // MSB
    uint8_t hum_adc_lsb; // LSB

    // Read registers
    API_BME680_readRegister(0x26, &hum_adc_msb, 1); // Lee el MSB
    API_BME680_readRegister(0x25, &hum_adc_lsb, 1); // Lee el LSB

    // Ensamblar el valor completo de hum_adc
    calib_data.hum_adc = ((uint32_t)hum_adc_msb << 8) | (uint32_t)hum_adc_lsb;

}

// Calibration of Gas
static void API_BME680_readCalibrationGas(void) {

    uint8_t buf[2];

    // Leer par_g1 (0xED)
    API_BME680_readRegister(0xED, buf, 1);
    calib_data.par_g1 = buf[0];

    // Leer par_g2 (0xEB / 0xEC)
    API_BME680_readRegister(0xEB, buf, 1);
    calib_data.par_g2 = buf[0];
    API_BME680_readRegister(0xEC, buf, 1);
    calib_data.par_g2 |= (int16_t)buf[0] << 8;

    // Leer par_g3 (0xEE)
    API_BME680_readRegister(0xEE, buf, 1);
    calib_data.par_g3 = (int8_t)buf[0];

    // Leer res_heat_range (0x02 <5:4>)
    API_BME680_readRegister(0x02, buf, 1);
    calib_data.res_heat_range = (buf[0] & 0x30) >> 4;

    // Leer res_heat_val (0x00)
    API_BME680_readRegister(0x00, buf, 1);
    calib_data.res_heat_val = (int8_t)buf[0];

    // Leer gas_adc (0x2B / 0x2A / 0x29)
    uint8_t gas_adc_msb;
    uint8_t gas_adc_mid;
    uint8_t gas_adc_lsb;

    // Read registers
    API_BME680_readRegister(0x2B, &gas_adc_msb, 1); // MSB
    API_BME680_readRegister(0x2A, &gas_adc_mid, 1); // Middle byte
    API_BME680_readRegister(0x29, &gas_adc_lsb, 1); // LSB

    // Ensamblar el valor completo de gas_adc
    calib_data.gas_adc = (gas_adc_msb << 10) | (gas_adc_mid << 2) | (gas_adc_lsb >> 6);

}

 // Read Chip ID
static uint8_t API_BME680_readChipID(void) {

	uint8_t reg = BME680_REG_CHIP_ID | 0x80;
	uint8_t chip_id = 0;

	API_BME680_selectPin(CS_Output_GPIO_Port, CS_Output_Pin);
	API_BME680_HAL_Transmit(&reg, sizeof(reg));
	API_BME680_HAL_Receive(&chip_id, sizeof(chip_id));
	API_BME680_deselectPin(CS_Output_GPIO_Port, CS_Output_Pin);

	return chip_id;

 }
