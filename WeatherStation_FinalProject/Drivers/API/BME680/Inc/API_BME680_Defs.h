/*
 * API_BME680_Refs.h
 *
 *  Created on: Aug 8, 2024
 *      Author: ubuntu
 */

#ifndef API_BME680_INC_API_BME680_DEFS_H_
#define API_BME680_INC_API_BME680_DEFS_H_

/* Private define ------------------------------------------------------------*/

// Masks

#define BME680_SPI_READ_MASK       0x80
#define BME680_SPI_WRITE_MASK      0x7F

// Memory page 0

// Chip ID
#define BME680_REG_CHIP_ID         0x50
#define BME680_REG_CHIP_RESET      0x60

// Memory page 1

// Initialization (Quick Start commands)
#define BME680_REG_CTRL_HUM        0x72  // Humidity control
#define BME680_REG_CTRL_MEAS       0x74  // Control de medición (temp y presión)
#define BME680_REG_CONFIG          0x75  // General configuration
#define BME680_REG_CTRL_GAS_1      0x71  // Gas measure control
#define BME680_REG_CTRL_GAS_0      0x70  // Gas sensor configuration
#define BME680_REG_GAS_WAIT_0      0x64  // Duración del calentamiento del sensor de gas
#define BME680_REG_RES_HEAT_0      0x5A  // Configuración del calentador del sensor de gas

#define BME680_MODE_SLEEP          0x00
#define BME680_MODE_FORCED         0x01

// Temperature
#define BME680_REG_TEMP_MSB        0x22
#define BME680_REG_TEMP_LSB        0x23
#define BME680_REG_TEMP_XLSB       0x24

// Pressure
#define BME680_REG_PRESS_MSB       0x1F
#define BME680_REG_PRESS_LSB       0x20
#define BME680_REG_PRESS_XLSB      0x21
// Humidity
#define BME680_REG_HUM_MSB         0x25
#define BME680_REG_HUM_LSB         0x26
// Gas
#define BME680_REG_GAS_LSB         0x2B
#define BME680_REG_GAS_MSB         0x2A

// Addresses for temp_comp calculation
// par_t1
#define BME680_REG_PAR_T1_LSB      0xE9
#define BME680_REG_PAR_T1_MSB      0xEA
// par_t2
#define BME680_REG_PAR_T2_LSB      0x8A
#define BME680_REG_PAR_T2_MSB      0x8B
// par_t3
#define BME680_REG_PAR_T3          0x8C
// temp_adc (0x24<7:4> / 0x23 / 0x22)
#define BME680_REG_TEMP_ADC_XLSB   0x24
#define BME680_REG_TEMP_ADC_LSB    0x23
#define BME680_REG_TEMP_ADC_MSB    0x22

// Addresses for press_comp calculation
// par_p1
#define BME680_REG_PAR_P1_LSB      0x8E
#define BME680_REG_PAR_P1_MSB      0x8F
// par_p2
#define BME680_REG_PAR_P2_LSB      0x90
#define BME680_REG_PAR_P2_MSB      0x91
// par_p3
#define BME680_REG_PAR_P3          0x92
// par_p4
#define BME680_REG_PAR_P4_LSB      0x94
#define BME680_REG_PAR_P4_MSB      0x95
// par_p5
#define BME680_REG_PAR_P5_LSB      0x96
#define BME680_REG_PAR_P5_MSB      0x97
// par_p6
#define BME680_REG_PAR_P6          0x99
// par_p7
#define BME680_REG_PAR_P7          0x98
// par_p8
#define BME680_REG_PAR_P8_LSB      0x9C
#define BME680_REG_PAR_P8_MSB      0x9D
// par_p9
#define BME680_REG_PAR_P9_LSB      0x9E
#define BME680_REG_PAR_P9_MSB      0x9F
// par_p10
#define BME680_REG_PAR_P10         0xA0
// press_adc (0x21<7:4> / 0x20 / 0x1F)
#define BME680_REG_PRESS_ADC_XLSB  0x21
#define BME680_REG_PRESS_ADC_LSB   0x20
#define BME680_REG_PRESS_ADC_MSB   0x1F

// Addresses for hum_comp calculation
// par_h1 (0xE2<3:0> / 0xE3)
#define BME680_REG_PAR_H1_LSB      0xE2
#define BME680_REG_PAR_H1_MSB      0xE3
// par_h2 (0xE2<7:4> / 0xE1)
#define BME680_REG_PAR_H2_LSB      0xE2
#define BME680_REG_PAR_H2_MSB      0xE1
// par_h3
#define BME680_REG_PAR_H3          0xE4
// par_h4
#define BME680_REG_PAR_H4          0xE5
// par_h5
#define BME680_REG_PAR_H5          0xE6
// par_h6
#define BME680_REG_PAR_H6          0xE7
// par_h7
#define BME680_REG_PAR_H7          0xE8
// hum_adc
#define BME680_REG_HUM_ADC_LSB     0x26
#define BME680_REG_HUM_ADC_MSB     0x25

// Addresses for gas_res calculation
// par_g1
#define BME680_REG_PAR_G1          0xED
// par_g2
#define BME680_REG_PAR_G2_LSB      0xEB
#define BME680_REG_PAR_G2_MSB      0xEC
// par_g3
#define BME680_REG_PAR_G3          0xEE
// res_heat_range (0x02 <5:4>)
#define BME680_REG_RES_HEAT_RG     0x02
// res_heat_val
#define BME680_REG_RES_HEAT_VAL    0x00
// gass_adc (0x2B<7:6> / 0x2A)
#define BME680_REG_GAS_ADC_LSB     0x2B
#define BME680_REG_GAS_ADC_MSB     0x2A
// gas_range (0x2B<3:0>)
#define BME680_REG_GAS_RG          0x2B
// range_switching_error
#define BME680_REG_RG_SWITCH_ERR   0x04

/*
 * @brief Structure to hold the calibration coefficients
 * TAKEN FROM BOSCH GITHUB (But modified)
 * https://github.com/boschsensortec/BME68x_SensorAPI/blob/master/bme68x_defs.h (LINE 771 TO 859)
 */
typedef struct {

    /*! Calibration coefficients for the humidity sensor */
    uint16_t par_h1;

    uint16_t par_h2;

    int8_t par_h3;

    int8_t par_h4;

    int8_t par_h5;

    uint8_t par_h6;

    int8_t par_h7;

    int32_t hum_adc;

    /*! Calibration coefficients for the gas sensor */
    int8_t par_gh1;

    int16_t par_gh2;

    int8_t par_gh3;

    int32_t gas_adc;

    /*! Calibration coefficients for the temperature sensor */
    uint16_t par_t1;

    int16_t par_t2;

    int8_t par_t3;

    int16_t temp_adc;

    /*! Calibration coefficients for the pressure sensor */
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

    /*! Variable to store the intermediate temperature coefficient */
    int32_t t_fine;

    /*! Heater resistance range coefficient */
    uint8_t res_heat_range;

    /*! Heater resistance value coefficient */
    int8_t res_heat_val;

    /*! Gas resistance range switching error coefficient */
    int8_t range_sw_err;

} BME680_CalibData;

#endif /* API_BME680_INC_API_BME680_DEFS_H_ */
