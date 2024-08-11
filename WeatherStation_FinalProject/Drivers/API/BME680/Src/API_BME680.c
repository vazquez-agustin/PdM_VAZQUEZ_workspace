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
/////////////////////
BME680_CalibData calib;

static uint8_t API_BME680_readChipID(void);
static void API_BME680_readRegister(uint8_t reg, uint8_t *data);
static void API_BME680_writeRegister(uint8_t reg, uint8_t value);

/* Public API code -----------------------------------------------------------*/

/* Function Implementations --------------------------------------------------*/

void API_BME680_setMemoryPage(uint8_t page) {

	uint8_t reg = 0x73 & BME680_SPI_WRITE_MASK; // Registro para seleccionar la página de memoria
	uint8_t value = (page == 1) ? 0x10 : 0x00; // Página 1 si page == 1, de lo contrario página 0

	API_BME680_selectPin();
	API_BME680_HAL_Transmit(&reg, sizeof(reg));
	API_BME680_HAL_Transmit(&value, sizeof(value));
	API_BME680_deselectPin();

}

void API_BME680_forceMeasurement(void) {

	uint8_t ctrl_meas = (0x02 << 5) | (0x05 << 2); // osrs_t<2:0> = 0b010 (2x), osrs_p<2:0> = 0b101 (16x)
	uint8_t mode = BME680_MODE_FORCED;
	API_BME680_writeRegister(BME680_REG_CTRL_MEAS, ctrl_meas | mode);

}

void API_BME680_Initialize(void) {
	/*
	API_BME680_setMemoryPage(0);

	API_BME680_readChipID();

	//uint8_t ctrl_reset = 0xb6;
	//API_BME680_writeRegister(0x60, ctrl_reset);

	API_BME680_HAL_Delay(10);
	*/
	API_BME680_setMemoryPage(1);

	// 1. Configura la sobremuestreo de humedad a 1x
	uint8_t ctrl_hum = (0x01);  // osrs_h<2:0> = 0b001 (1x)
	API_BME680_writeRegister(BME680_REG_CTRL_HUM, ctrl_hum);

	// 2. Configura la sobremuestreo de temperatura a 2x y presión a 16x
	uint8_t ctrl_meas = (0x02 << 5) | (0x05 << 2); // osrs_t<2:0> = 0b010 (2x), osrs_p<2:0> = 0b101 (16x)
	//API_BME680_writeRegister(0x75, 0);
	API_BME680_writeRegister(BME680_REG_CTRL_MEAS, ctrl_meas);
	/*
	// 3. Configura la duración del calentamiento del sensor de gas a 100 ms
	uint8_t gas_wait_0 = 0x59;  // 100 ms heat up
	API_BME680_writeRegister(BME680_REG_GAS_WAIT_0, gas_wait_0);

	// 4. Configura el set-point del calentador a 300 °C (esto es un ejemplo, la conversión puede variar)
	uint8_t reg_value = (uint8_t) ((300 / 10) + 46);
	uint8_t res_heat_0 = reg_value; // Conversión a valor de registro
	API_BME680_writeRegister(BME680_REG_RES_HEAT_0, res_heat_0);

	// 5. Selecciona los parámetros del calentador y activa la medición de gas
	uint8_t ctrl_gas_1 = (0x00 & 0x0F) | (1 << 4); // nb_conv<3:0> = 0x0, run_gas = 1
	API_BME680_writeRegister(BME680_REG_CTRL_GAS_1, ctrl_gas_1);
	*/
	// 6. Establece el modo en "forced mode" para iniciar la medición
	uint8_t mode = BME680_MODE_FORCED;
	API_BME680_writeRegister(BME680_REG_CTRL_MEAS, ctrl_meas | mode);


}

// Temperature
uint32_t API_BME680_readTempADC(void) {

	API_BME680_setMemoryPage(1);  // Cambia a la página de memoria 1

	uint8_t t_reg_xlsb, t_reg_lsb, t_reg_msb;

	API_BME680_readRegister(BME680_REG_TEMP_ADC_XLSB, &t_reg_xlsb);
	API_BME680_readRegister(BME680_REG_TEMP_ADC_LSB, &t_reg_lsb);
	API_BME680_readRegister(BME680_REG_TEMP_ADC_MSB, &t_reg_msb);

	uint8_t t_xlsb = 0, t_lsb = 0, t_msb = 0;
	uint32_t temp_adc = 0;

	// Read XLSB
	API_BME680_selectPin();
	API_BME680_HAL_Transmit(&t_reg_xlsb, sizeof(t_reg_xlsb));
	API_BME680_HAL_Receive(&t_xlsb, sizeof(t_xlsb));
	API_BME680_deselectPin();

	// Read LSB
	API_BME680_selectPin();
	API_BME680_HAL_Transmit(&t_reg_lsb, sizeof(t_reg_lsb));
	API_BME680_HAL_Receive(&t_lsb, sizeof(t_lsb));
	API_BME680_deselectPin();

	// Read MSB
	API_BME680_selectPin();
	API_BME680_HAL_Transmit(&t_reg_msb, sizeof(t_reg_msb));
	API_BME680_HAL_Receive(&t_msb, sizeof(t_msb));
	API_BME680_deselectPin();

	// Combina los bytes en el orden correcto
	temp_adc = (((uint32_t) t_msb) << 12) | (((uint32_t) t_lsb) << 4) | (((uint32_t) t_xlsb) >> 4);

	return temp_adc & 0x1ffff;

}

int16_t API_BME680_calculateTemperature(uint32_t temp_adc) {

	int32_t var1, var2, var3, temp_comp;

	var1 = ((int32_t) temp_adc >> 3) - ((int32_t) calib.par_t1 << 1);
	var2 = (var1 * (int32_t) calib.par_t2) >> 11;
	var3 = ((((var1 >> 1) * (var1 >> 1)) >> 12) * ((int32_t) calib.par_t3 << 4)) >> 14;
	calib.t_fine = var2 + var3;
	temp_comp = ((calib.t_fine * 5) + 128) >> 8;

	//
	return temp_comp;

}

// Pressure
uint32_t API_BME680_readPressADC(void) {

	API_BME680_setMemoryPage(1);  // Cambia a la página de memoria 1

	uint8_t p_reg_xlsb, p_reg_lsb, p_reg_msb;

	API_BME680_readRegister(BME680_REG_PRESS_ADC_XLSB, &p_reg_xlsb);
	API_BME680_readRegister(BME680_REG_PRESS_ADC_LSB, &p_reg_lsb);
	API_BME680_readRegister(BME680_REG_PRESS_ADC_MSB, &p_reg_msb);

	uint8_t p_xlsb = 0, p_lsb = 0, p_msb = 0;
	uint32_t press_adc = 0;

	// Read XLSB
	API_BME680_selectPin();
	API_BME680_HAL_Transmit(&p_reg_xlsb, sizeof(p_reg_xlsb));
	API_BME680_HAL_Receive(&p_xlsb, sizeof(p_xlsb));
	API_BME680_deselectPin();

	// Read LSB
	API_BME680_selectPin();
	API_BME680_HAL_Transmit(&p_reg_lsb, sizeof(p_reg_lsb));
	API_BME680_HAL_Receive(&p_lsb, sizeof(p_lsb));
	API_BME680_deselectPin();

	// Read MSB
	API_BME680_selectPin();
	API_BME680_HAL_Transmit(&p_reg_msb, sizeof(p_reg_msb));
	API_BME680_HAL_Receive(&p_msb, sizeof(p_msb));
	API_BME680_deselectPin();

	// Combina los bytes en el orden correcto
	press_adc = (((uint32_t) p_msb) << 12) | (((uint32_t) p_lsb) << 4) | (((uint32_t) p_xlsb) >> 4);

	return press_adc;
}

double API_BME680_calculatePressure(uint32_t press_adc) {

	int32_t var1, var2, var3, press_comp;

	var1 = ((int32_t) calib.t_fine >> 1) - 64000;
	var2 = ((((var1 >> 2) * (var1 >> 2)) >> 11) * (int32_t) calib.par_p6) >> 2;
	var2 = var2 + ((var1 * (int32_t) calib.par_p5) << 1);
	var2 = (var2 >> 2) + ((int32_t) calib.par_p4 << 16);
	var1 = (((((var1 >> 2) * (var1 >> 2)) >> 13) *
			   ((int32_t) calib.par_p3 << 5)) >> 3) + (((int32_t) calib.par_p2 * var1) >> 1);
	var1 = var1 >> 18;
	var1 = ((32768 + var1) * (int32_t)calib.par_p1) >> 15;
	press_comp = 1048576 - press_adc;
	press_comp = (uint32_t)((press_comp - (var2 >> 12)) * ((uint32_t)3125));
	if (press_comp >= (1 << 30)) {
		press_comp = ((press_comp / (uint32_t)var1) << 1);
	} else {
		press_comp = ((press_comp << 1) / (uint32_t)var1);
	}
	var1 = ((int32_t)calib.par_p9 * (int32_t)(((press_comp >> 3) *
			    (press_comp >> 3)) >> 13)) >> 12;
	var2 = ((int32_t)(press_comp >> 2) * (int32_t)calib.par_p8) >> 13;
	var3 = ((int32_t)(press_comp >> 8) * (int32_t)(press_comp >> 8) *
			    (int32_t)(press_comp >> 8) * (int32_t)calib.par_p10) >> 17;
	press_comp = (int32_t)(press_comp) +
				((var1 + var2 + var3 + ((int32_t)calib.par_p7 << 7)) >> 4);

	return press_comp / 100.0; // Conversion from Pa to hPa

}

// Humidity
uint32_t API_BME680_readHumADC(void) {

	API_BME680_setMemoryPage(1);  // Cambia a la página de memoria 1

	uint8_t h_reg_lsb, h_reg_msb;

	API_BME680_readRegister(BME680_REG_HUM_ADC_LSB, &h_reg_lsb);
	API_BME680_readRegister(BME680_REG_HUM_ADC_MSB, &h_reg_msb);

	uint8_t h_lsb = 0, h_msb = 0;
	uint32_t hum_adc = 0;

	// Read LSB
	API_BME680_selectPin();
	API_BME680_HAL_Transmit(&h_reg_lsb, sizeof(h_reg_lsb));
	API_BME680_HAL_Receive(&h_lsb, sizeof(h_lsb));
	API_BME680_deselectPin();

	// Read MSB
	API_BME680_selectPin();
	API_BME680_HAL_Transmit(&h_reg_msb, sizeof(h_reg_msb));
	API_BME680_HAL_Receive(&h_msb, sizeof(h_msb));
	API_BME680_deselectPin();

	// Combina los bytes en el orden correcto
	hum_adc = (((uint32_t) h_msb) << 8) | (uint32_t) h_lsb;

	return hum_adc;

}

double API_BME680_calculateHumidity(uint32_t hum_adc, uint32_t temp_comp) {

	int32_t var1, var2, var3, var4, var5, var6, hum_comp, temp_scaled;

	temp_scaled = (int32_t)temp_comp;
	var1 = (int32_t)hum_adc - (int32_t)((int32_t)calib.par_h1 << 4) -
			(((temp_scaled * (int32_t)calib.par_h3) / ((int32_t)100)) >> 1);
	var2 = ((int32_t)calib.par_h2 * (((temp_scaled *
			(int32_t)calib.par_h4) / ((int32_t)100)) +
			(((temp_scaled * ((temp_scaled * (int32_t)calib.par_h5) /
			((int32_t)100))) >> 6) / ((int32_t)100)) + ((int32_t)(1 << 14)))) >> 10;
	var3 = var1 * var2;
	var4 = (((int32_t)calib.par_h6 << 7) +
			((temp_scaled * (int32_t)calib.par_h7) / ((int32_t)100))) >> 4;
	var5 = ((var3 >> 14) * (var3 >> 14)) >> 10;
	var6 = (var4 * var5) >> 1;
	hum_comp = (((var3 + var6) >> 10) * ((int32_t) 1000)) >> 12;

	return hum_comp;

}

// Función para leer y almacenar los datos de calibración de temperatura
void API_BME680_readCalibrationData(void) {

	// Temperature calibration
	// LSB
	uint8_t t1_lsb, t2_lsb;
	// MSB
	uint8_t t1_msb, t2_msb;
	uint8_t t3;

	// Read temperature calibration registers
	API_BME680_readRegister(BME680_REG_PAR_T1_LSB, &t1_lsb);
	API_BME680_readRegister(BME680_REG_PAR_T1_MSB, &t1_msb);
	calib.par_t1 = (uint16_t) ((t1_msb << 8) | t1_lsb);

	API_BME680_readRegister(BME680_REG_PAR_T2_LSB, &t2_lsb);
	API_BME680_readRegister(BME680_REG_PAR_T2_MSB, &t2_msb);
	calib.par_t2 = (int16_t) ((t2_msb << 8) | t2_lsb);

	API_BME680_readRegister(BME680_REG_PAR_T3, &t3);
	calib.par_t3 = (int8_t) t3;

	// Pressure calibration
	// LSB
	uint8_t p1_lsb, p2_lsb, p4_lsb, p5_lsb, p8_lsb, p9_lsb;
	// MSB
	uint8_t p1_msb, p2_msb, p4_msb, p5_msb, p8_msb, p9_msb;
	uint8_t p3, p6, p7, p10;

	// Read pressure calibration registers
	API_BME680_readRegister(BME680_REG_PAR_P1_LSB, &p1_lsb);
	API_BME680_readRegister(BME680_REG_PAR_P1_MSB, &p1_msb);
	calib.par_p1 = (uint16_t) ((p1_msb << 8) | p1_lsb);

	API_BME680_readRegister(BME680_REG_PAR_P2_LSB, &p2_lsb);
	API_BME680_readRegister(BME680_REG_PAR_P2_MSB, &p2_msb);
	calib.par_p2 = (int16_t) ((p2_msb << 8) | p2_lsb);

	API_BME680_readRegister(BME680_REG_PAR_P3, &p3);
	calib.par_p3 = (int8_t) p3;

	API_BME680_readRegister(BME680_REG_PAR_P4_LSB, &p4_lsb);
	API_BME680_readRegister(BME680_REG_PAR_P4_MSB, &p4_msb);
	calib.par_p4 = (int16_t) ((p4_msb << 8) | p4_lsb);

	API_BME680_readRegister(BME680_REG_PAR_P5_LSB, &p5_lsb);
	API_BME680_readRegister(BME680_REG_PAR_P5_MSB, &p5_msb);
	calib.par_p5 = (int16_t) ((p5_msb << 8) | p5_lsb);

	API_BME680_readRegister(BME680_REG_PAR_P6, &p6);
	calib.par_p6 = (int8_t) p6;

	API_BME680_readRegister(BME680_REG_PAR_P7, &p7);
	calib.par_p7 = (int8_t) p7;

	API_BME680_readRegister(BME680_REG_PAR_P8_LSB, &p8_lsb);
	API_BME680_readRegister(BME680_REG_PAR_P8_MSB, &p8_msb);
	calib.par_p8 = (int16_t) ((p8_msb << 8) | p8_lsb);

	API_BME680_readRegister(BME680_REG_PAR_P9_LSB, &p9_lsb);
	API_BME680_readRegister(BME680_REG_PAR_P9_MSB, &p9_msb);
	calib.par_p9 = (int16_t) ((p9_msb << 8) | p9_lsb);

	API_BME680_readRegister(BME680_REG_PAR_P10, &p10);
	calib.par_p10 = (uint8_t) p10;

	// Humidity calibration
	// LSB
	uint8_t h1_lsb, h2_lsb;
	// MSB
	uint8_t h1_msb, h2_msb;
	uint8_t h3, h4, h5, h6, h7;

	// Read pressure calibration registers
	API_BME680_readRegister(BME680_REG_PAR_H1_LSB, &h1_lsb);
	API_BME680_readRegister(BME680_REG_PAR_H1_MSB, &h1_msb);
	calib.par_h1 = (uint16_t) ((h1_msb << 8) | h1_lsb);

	API_BME680_readRegister(BME680_REG_PAR_H2_LSB, &h2_lsb);
	API_BME680_readRegister(BME680_REG_PAR_H2_MSB, &h2_msb);
	calib.par_h2 = (int16_t) ((h2_msb << 8) | h2_lsb);

	API_BME680_readRegister(BME680_REG_PAR_H3, &h3);
	calib.par_h3 = (uint8_t) h3;

	API_BME680_readRegister(BME680_REG_PAR_H4, &h4);
	calib.par_h4 = (uint8_t) h4;

	API_BME680_readRegister(BME680_REG_PAR_H5, &h5);
	calib.par_h5 = (uint8_t) h5;

	API_BME680_readRegister(BME680_REG_PAR_H6, &h6);
	calib.par_h6 = (uint8_t) h6;

	API_BME680_readRegister(BME680_REG_PAR_H7, &h7);
	calib.par_h7 = (uint8_t) h7;

}

/* Private API code ----------------------------------------------------------*/

/* Function Implementations --------------------------------------------------*/

static void API_BME680_writeRegister(uint8_t reg, uint8_t value) {

	reg &= BME680_SPI_WRITE_MASK; // Asegúrate de que el bit de escritura esté limpio

	API_BME680_selectPin();
	API_BME680_HAL_Transmit(&reg, sizeof(reg));
	API_BME680_HAL_Transmit(&value, sizeof(value)); //////////
	API_BME680_deselectPin();

	API_BME680_HAL_Delay(10);

}

static void API_BME680_readRegister(uint8_t reg, uint8_t *data) {

	reg |= BME680_SPI_READ_MASK;

	API_BME680_selectPin();
	API_BME680_HAL_Transmit(&reg, 1);
	API_BME680_HAL_Receive(data, 1);
	API_BME680_deselectPin();

}


 // Initialize BME680 by reading ChipID
 void API_BME680_Init(void) {

 API_BME680_HAL_SPI_Init();
 API_BME680_HAL_Delay(1000);

 API_BME680_readChipID();

 }

 // Read Chip ID ESTA BIEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEN
 static uint8_t API_BME680_readChipID(void) {

 //API_BME680_setMemoryPage(0);

 uint8_t reg = BME680_REG_CHIP_ID | 0x80;
 uint8_t chip_id = 0;

 API_BME680_selectPin();
 API_BME680_HAL_Transmit(&reg, sizeof(reg));
 API_BME680_HAL_Receive(&chip_id, sizeof(chip_id));
 API_BME680_deselectPin();

 return chip_id;

 }

