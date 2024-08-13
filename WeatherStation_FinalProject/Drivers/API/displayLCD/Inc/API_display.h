/*
 * API_display.h
 *
 *  Created on: Jul 29, 2024
 *      Author: Agustin Vazquez
 */

#ifndef API_DISPLAYLCD_INC_API_DISPLAY_H_
#define API_DISPLAYLCD_INC_API_DISPLAY_H_

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* Public define -------------------------------------------------------------*/

#define LINE1    0
#define LINE2    1

/* Exported functions prototypes ---------------------------------------------*/

void API_display_Init(void);
void API_display_Clear(void);
void API_display_sendChar(uint8_t character); //ascii = char
void API_display_sendString(char *str);
void API_display_setCursor(uint8_t row, uint8_t col);

#endif /* API_DISPLAYLCD_INC_API_DISPLAY_H_ */
