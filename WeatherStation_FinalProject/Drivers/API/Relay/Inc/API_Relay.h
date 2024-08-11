/*
 * API_Relay.h
 *
 *  Created on: Aug 8, 2024
 *      Author: ubuntu
 */

#ifndef API_RELAY_INC_API_RELAY_H_
#define API_RELAY_INC_API_RELAY_H_

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* Function Prototypes -------------------------------------------------------*/

void API_Relay_Init(void);
void API_Relay_On(void);
void API_Relay_Off(void);
void API_Relay_Toggle(void);

#endif /* API_RELAY_INC_API_RELAY_H_ */
