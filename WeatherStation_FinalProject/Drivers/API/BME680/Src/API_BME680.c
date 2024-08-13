/*
 * API_BME680.c
 *
 *  Created on: Aug 3, 2024
 *      Author: Agustin Vazquez
 */

/* Includes ------------------------------------------------------------------*/

#include "API_BME680.h"
#include "API_BME680_HAL.h"
#include "API_display.h"

/* Private define ------------------------------------------------------------*/

BME680_CalibData calib;

static void API_BME680_readRegister(uint8_t reg, uint8_t *data);
static void API_BME680_writeRegister(uint8_t reg, uint8_t value);

/* Public API code -----------------------------------------------------------*/

/* Function Implementations --------------------------------------------------*/

/**
  * @brief  Sets the memory page for the BME680 sensor.
  * @param  page: Memory page to set (0 or 1).
  * @retval None.
  */
void API_BME680_setMemoryPage(uint8_t page) {

	uint8_t reg = BME680_REG_MEM_PAGE1 & BME680_SPI_WRITE_MASK;
	// In SPI mode complete memory map is accessed
	// using page 0 and page 1. After power-on,
	// spi_mem_page is in its reset state and page 0
	// be active. To access to control registers of
	// temperature, humidity & pressure, is necessary
	// to change to page 1.
	uint8_t value = (page == 1) ? BME680_PAGE1 : BME680_PAGE2;

	API_BME680_selectPin();
	API_BME680_HAL_Transmit(&reg, sizeof(reg));
	API_BME680_HAL_Transmit(&value, sizeof(value));
	API_BME680_deselectPin();

}

/**
  * @brief  Initializes the BME680 sensor with default settings.
  * @retval None.
  */
void API_BME680_Initialize(void) {

	API_BME680_setMemoryPage(1);

	// Select oversampling for T, P and H
	// Set osrs_x<2:0>
	uint8_t ctrl_hum = (0x01);  // osrs_h<2:0> = 0b001 (1x)
	API_BME680_writeRegister(BME680_REG_CTRL_HUM, ctrl_hum);

	uint8_t ctrl_meas = (0x02 << 5) | (0x05 << 2); // osrs_t<2:0> = 0b010 (2x), osrs_p<2:0> = 0b101 (16x)
	API_BME680_writeRegister(BME680_REG_CTRL_MEAS, ctrl_meas);

	// Here would be next steps to set gas sensor

	// Set mode to forced mode
	// Set mode<1:0> to 0b01
	uint8_t mode = BME680_MODE_FORCED;
	API_BME680_writeRegister(BME680_REG_CTRL_MEAS, ctrl_meas | mode);

}

//                               TEMPERATURE

/**
  * @brief  Reads the raw ADC value of the temperature from the BME680 sensor.
  * @retval uint32_t: The raw ADC temperature value (17-bit resolution).
  */
uint32_t API_BME680_readTempADC(void) {

	API_BME680_setMemoryPage(1);

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

	temp_adc = (((uint32_t) t_msb) << 12) | (((uint32_t) t_lsb) << 4) | (((uint32_t) t_xlsb) >> 4);

	return temp_adc & 0x1ffff; // Mask to adjust 17 bits resolution (according to osrs_t = 2)

}

/**
  * @brief  Calculates the compensated temperature from the raw ADC value.
  * @param  temp_adc: Raw ADC value of the temperature.
  * @retval int16_t: The compensated temperature in degrees Celsius.
  */
int16_t API_BME680_calculateTemperature(uint32_t temp_adc) {

	int32_t var1, var2, var3, temp_comp;

	var1 = ((int32_t) temp_adc >> 3) - ((int32_t) calib.par_t1 << 1);
	var2 = (var1 * (int32_t) calib.par_t2) >> 11;
	var3 = ((((var1 >> 1) * (var1 >> 1)) >> 12) * ((int32_t) calib.par_t3 << 4)) >> 14;
	calib.t_fine = var2 + var3;
	temp_comp = ((calib.t_fine * 5) + 128) >> 8;

	return temp_comp / 100;

}

//                               PRESSURE

/**
  * @brief  Reads the raw ADC value of the pressure from the BME680 sensor.
  * @retval uint32_t: The raw ADC pressure value.
  */
uint32_t API_BME680_readPressADC(void) {

	API_BME680_setMemoryPage(1);

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

	press_adc = (((uint32_t) p_msb) << 12) | (((uint32_t) p_lsb) << 4) | (((uint32_t) p_xlsb) >> 4);

	return press_adc;
}

/**
  * @brief  Calculates the compensated pressure from the raw ADC value.
  * @param  press_adc: Raw ADC value of the pressure.
  * @retval int16_t: The compensated pressure in hPa (hectopascals).
  */
int16_t API_BME680_calculatePressure(uint32_t press_adc) {

	int32_t var1, var2, var3, press_comp;

	var1 = ((int32_t) calib.t_fine >> 1) - 64000;
	var2 = ((((var1 >> 2) * (var1 >> 2)) >> 11) * (int32_t) calib.par_p6) >> 2;
	var2 = var2 + ((var1 * (int32_t) calib.par_p5) << 1);
	var2 = (var2 >> 2) + ((int32_t) calib.par_p4 << 16);
	var1 = (((((var1 >> 2) * (var1 >> 2)) >> 13) *
			((int32_t) calib.par_p3 << 5)) >> 3) + (((int32_t) calib.par_p2 * var1) >> 1);
	var1 = var1 >> 18;
	var1 = ((32768 + var1) * (int32_t) calib.par_p1) >> 15;
	press_comp = 1048576 - press_adc;
	press_comp = (uint32_t) ((press_comp - (var2 >> 12)) * ((uint32_t) 3125));
	if (press_comp >= (1 << 30)) {
		press_comp = ((press_comp / (uint32_t) var1) << 1);
	} else {
		press_comp = ((press_comp << 1) / (uint32_t) var1);
	}

	var1 = ((int32_t) calib.par_p9 * (int32_t) (((press_comp >> 3) *
				(press_comp >> 3)) >> 13)) >> 12;
	var2 = ((int32_t) (press_comp >> 2) * (int32_t) calib.par_p8) >> 13;
	var3 = ((int32_t) (press_comp >> 8) * (int32_t) (press_comp >> 8) *
				(int32_t) (press_comp >> 8) * (int32_t) calib.par_p10) >> 17;
	press_comp = (int32_t) (press_comp)	+ ((var1 + var2 + var3 + ((int32_t) calib.par_p7 << 7)) >> 4);

	return press_comp / 100; // Conversion from Pa to hPa

}

//                               HUMIDITY

/**
  * @brief  Reads the raw ADC value of the humidity from the BME680 sensor.
  * @retval uint32_t: The raw ADC humidity value.
  */
uint32_t API_BME680_readHumADC(void) {

	API_BME680_setMemoryPage(1);

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

	hum_adc = (((uint32_t) h_msb) << 8) | (uint32_t) h_lsb;

	return hum_adc;

}

/**
  * @brief  Calculates the compensated humidity from the raw ADC value.
  * @param  hum_adc: Raw ADC value of the humidity.
  * @param  temp_comp: Compensated temperature value, used in humidity calculation.
  * @retval int16_t: The compensated humidity in %RH (relative humidity).
  */
int16_t API_BME680_calculateHumidity(uint32_t hum_adc, uint32_t temp_comp) {

	int32_t var1, var2, var3, var4, var5, var6, hum_comp, temp_scaled;

	temp_scaled = (int32_t) temp_comp;
	var1 = (int32_t) hum_adc - (int32_t) ((int32_t) calib.par_h1 << 4) -
			(((temp_scaled * (int32_t) calib.par_h3) / ((int32_t) 100)) >> 1);
	var2 = ((int32_t) calib.par_h2 * (((temp_scaled *
			(int32_t) calib.par_h4) / ((int32_t) 100)) +
			(((temp_scaled * ((temp_scaled * (int32_t) calib.par_h5) /
			((int32_t) 100))) >> 6) / ((int32_t) 100)) + ((int32_t) (1 << 14)))) >> 10;
	var3 = var1 * var2;
	var4 = (((int32_t) calib.par_h6 << 7)+
			((temp_scaled * (int32_t) calib.par_h7) / ((int32_t) 100))) >> 4;
	var5 = ((var3 >> 14) * (var3 >> 14)) >> 10;
	var6 = (var4 * var5) >> 1;
	hum_comp = (((var3 + var6) >> 10) * ((int32_t) 1000)) >> 12;

	return hum_comp;

}

/**
  * @brief  Reads the calibration data from the BME680 sensor's registers.
  * @retval None.
  */
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

/**
  * @brief  Writes a value to a specific register of the BME680 sensor.
  * @param  reg: Register address to write to.
  * @param  value: Value to write to the register.
  * @retval None.
  */
static void API_BME680_writeRegister(uint8_t reg, uint8_t value) {

	reg &= BME680_SPI_WRITE_MASK;

	API_BME680_selectPin();
	API_BME680_HAL_Transmit(&reg, sizeof(reg));
	API_BME680_HAL_Transmit(&value, sizeof(value));
	API_BME680_deselectPin();

}

/**
  * @brief  Reads a value from a specific register of the BME680 sensor.
  * @param  reg: Register address to read from.
  * @param  data: Pointer to the variable where the read data will be stored.
  * @retval None.
  */
static void API_BME680_readRegister(uint8_t reg, uint8_t *data) {

	reg |= BME680_SPI_READ_MASK;

	API_BME680_selectPin();
	API_BME680_HAL_Transmit(&reg, 1);
	API_BME680_HAL_Receive(data, 1);
	API_BME680_deselectPin();

}
