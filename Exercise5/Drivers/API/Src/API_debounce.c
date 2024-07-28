/*
 * API_debounce.c
 *
 *  Created on: Jul 18, 2024
 *      Author: laura
 */

/* Includes ------------------------------------------------------------------*/
#include "API_debounce.h"

/* Private variables ---------------------------------------------------------*/
typedef enum{

BUTTON_UP,  // Estado: inicial (no presionado)
BUTTON_FALLING,  // Estado: siendo presionado
BUTTON_DOWN, // Estado: totalmente presionado
BUTTON_RAISING, // Estado: se suelta el botón

} debounceState_t;

// Variable global privado
static bool_t button_pressed;

// Variable global privada (con static)
static debounceState_t currentState;

/* Private function prototypes --------------------------*/
void debounceFSM_init(void){

	currentState = BUTTON_UP;
	button_pressed = false;

}


bool_t debounceFSM_isButtonDown() {

	return (currentState == BUTTON_DOWN);

}

// Esta función devuelve el valor de button_pressed. TRUE significa que button == pressed
bool_t debounceFSM_readKey() {

	if (button_pressed) {

		button_pressed = false;
		return true;

	}

	return button_pressed;

}

void debounceFSM_update(bool_t button) {

	button_pressed = button;

	switch(currentState) {

	case BUTTON_UP:

		if (debounceFSM_readKey()) {

			currentState = BUTTON_FALLING;

		}

		break;

	case BUTTON_FALLING:

		if (debounceFSM_readKey()) {

			currentState = BUTTON_DOWN;

		} else {

			currentState = BUTTON_UP;

		}

		break;

	case BUTTON_DOWN:

		if (!debounceFSM_readKey()) {

			currentState = BUTTON_RAISING;

		}

		break;

	case BUTTON_RAISING:

		if (!debounceFSM_readKey()) {

			currentState = BUTTON_UP;

		} else {

			currentState = BUTTON_DOWN;
		}

		break;

	default:

		debounceFSM_init();

		break;

	}

}
