/*
 * API_display.h
 *
 *  Created on: Jul 29, 2024
 *      Author: ubuntu
 */

#ifndef API_DISPLAYLCD_INC_API_DISPLAY_H_
#define API_DISPLAYLCD_INC_API_DISPLAY_H_

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* Exported functions prototypes ---------------------------------------------*/

void API_display_Init(void);
void API_display_Clear(void);
void API_display_SendChar(uint8_t character); //ascii = char
void API_display_SendString(char *str);
void API_display_SetCursor(uint8_t row, uint8_t col);

#endif /* API_DISPLAYLCD_INC_API_DISPLAY_H_ */
