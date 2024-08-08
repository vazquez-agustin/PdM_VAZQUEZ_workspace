/*
 * API_Relay.c
 *
 *  Created on: Aug 8, 2024
 *      Author: ubuntu
 */

/* Includes ------------------------------------------------------------------*/

#include "API_Relay.h"
#include "API_Relay_HAL.h"

/* Public API code -----------------------------------------------------------*/

/* Function Implementations --------------------------------------------------*/

// Función para inicializar el relé
void API_Relay_Init(void) {
    // Esta función ya debería estar llamada en MX_GPIO_Init(), pero la incluimos por completitud
	API_Relay_HAL_GPIO_Init();
}

// Función para activar el relé
void API_Relay_On(uint8_t port, uint8_t pin) {

	API_Relay_HAL_deselectPin(Relay_Output_GPIO_Port, Relay_Output_Pin);

}

// Función para desactivar el relé
void API_Relay_Off(uint8_t port, uint8_t pin) {

	API_Relay_HAL_selectPin(Relay_Output_GPIO_Port, Relay_Output_Pin);

}

// Función para alternar el estado del relé (si es necesario)
void API_Relay_Toggle(uint8_t port, uint8_t pin) {

	API_Relay_HAL_Toggle(Relay_Output_GPIO_Port, Relay_Output_Pin);

}


