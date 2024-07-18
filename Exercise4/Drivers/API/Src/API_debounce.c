
#include "API_debounce.h"

// Enum privado
typedef enum{

BUTTON_UP,  // Estado: inicial (no presionado)
BUTTON_FALLING,  // Estado: siendo presionado
BUTTON_DOWN, // Estado: totalmente presionado
BUTTON_RAISING, // Estado: se suelta el botón

} debounceState_t;

// Variable global privado
bool_t button_pressed;

// Variable global privada (con static)
static debounceState_t estadoActual;

void debounceFSM_init(void){

  	 estadoActual = BUTTON_UP;

  }

void writeKey() {

	button_pressed = true;

}

bool_t readKey() {

	if (button_pressed) {

		button_pressed = false;

		return true;

	}

	return button_pressed;

}

  void debounceFSM_update(void) {

  	  switch(estadoActual) {

  	  case BUTTON_UP:

  		  estadoActual = BUTTON_FALLING;
  		  break;

  	  case BUTTON_FALLING:


  		  break;

  	  case BUTTON_RAISING:

  		  break;

  	  case BUTTON_DOWN:

  		  break;

  	  default:


  	  }

    }




