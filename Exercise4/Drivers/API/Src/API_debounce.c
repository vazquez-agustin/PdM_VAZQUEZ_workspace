
#include "API_debounce.h"

  void debounceFSM_init(void){

  	 estadoActual = BUTTON_UP;

  }

  void debounceFSM_update(void) {

  	  switch(estadoActual) {

  	  case BUTTON_UP:

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




