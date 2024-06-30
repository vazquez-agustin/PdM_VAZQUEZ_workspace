
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#include <stdint.h>  // Necesario para uint32_t
#include <stdbool.h> // Necesario para bool



/* Exported types ------------------------------------------------------------*/
typedef uint32_t tick_t; // Representa un tipo de dato para almacenar un tiempo o conteo en milisegundos
typedef bool bool_t; // Representa un tipo de dato booleano

typedef struct {
   tick_t startTime; // Almacena el tiempo en milisegundos cuando se inició el retardo
   tick_t duration;  // Almacena la duración del retardo en milisegundos
   bool_t running;  // Es un booleano que indica si el retardo está en curso (true) o no (false)
} delay_t;

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/*
 * Inicializa la estructura delay_t con la duración especificada, configurando running a
 * false inicialmente.
 * */
void delayInit(delay_t *delay, tick_t duration);

/*
 * Verifica el estado del retardo. Si running es false, inicia el retardo y retorna false.
 * Si running es true, verifica si ha transcurrido la duración del retardo desde startTime y
 * retorna true si se ha cumplido el tiempo, reiniciando running a false en ese caso.
 * */
bool_t delayRead(delay_t *delay);

/*
 * Permite cambiar la duración de un retardo existente actualizando el campo duration en la
 * estructura delay_t.
 * */
void delayWrite(delay_t *delay, tick_t duration);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
