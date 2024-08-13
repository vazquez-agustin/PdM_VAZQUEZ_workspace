/*
 * API_delay.h
 *
 *  Created on: Jul 4, 2024
 *      Author: Agustin Vazquez
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef API_API_DELAY_H_
#define API_API_DELAY_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>

/* Exported types ------------------------------------------------------------*/

typedef uint32_t tick_t;
typedef bool bool_t;

typedef struct {
	tick_t startTime;  // Stores the time in milliseconds when the delay started
	tick_t duration;   // Stores the duration of the delay in milliseconds
	bool_t running;    // A boolean that indicates whether the delay is in progress (true) or not (false)
} delay_t;

/* Exported functions prototypes ---------------------------------------------*/

void delayInit(delay_t *delay, tick_t duration);
void delayWrite(delay_t *delay, tick_t duration);
bool_t delayRead(delay_t *delay);
bool_t delayIsRunning(delay_t *delay);

#ifdef __cplusplus
}
#endif

#endif /* API_API_DELAY_H_ */
