/*
 * API_display.h
 *
 *  Created on: Jul 29, 2024
 *      Author: ubuntu
 */

#ifndef API_DISPLAYLCD_INC_API_DISPLAY_H_
#define API_DISPLAYLCD_INC_API_DISPLAY_H_

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "API_display_HAL.h"

// Prototipos de funciones
void API_display_Init(I2C_HandleTypeDef *hi2c);
void API_display_Clear(I2C_HandleTypeDef *hi2c);
void API_display_SendCommand(I2C_HandleTypeDef *hi2c, uint8_t command);
void API_display_SendData (I2C_HandleTypeDef *hi2c, uint8_t data);
void API_display_Send_String (char *str);
void API_display_Set_Cursor(I2C_HandleTypeDef *hi2c, uint8_t row, uint8_t col);
void API_display_Print(I2C_HandleTypeDef *hi2c, const char *str);



#endif /* API_DISPLAYLCD_INC_API_DISPLAY_H_ */
