/*
 * API_debounce.h
 *
 *  Created on: Jul 12, 2024
 *      Author: ubuntu
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

typedef enum{

BUTTON_UP,  // Estado: inicial (no presionado)
BUTTON_FALLING,  // Estado: siendo presionado
BUTTON_DOWN, // Estado: totalmente presionado
BUTTON_RAISING, // Estado: se suelta el botón

} debounceState_t;


void debounceFSM_init();

void debounceFSM_update();


debounceState_t estadoActual;


#endif /* API_INC_API_DEBOUNCE_H_ */
