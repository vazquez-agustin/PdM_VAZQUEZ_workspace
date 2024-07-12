/* Includes ------------------------------------------------------------------*/
#include "API_delay.h"
#include "stm32f4xx_hal.h"


/* Private functions ---------------------------------------------------------*/
/**
 * @brief Inicializa la estructura de retardo no bloqueante.
 *
 * Esta función inicializa una estructura `delay_t` con la duración especificada y
 * establece el campo `running` en `false`, es decir, que el retardo aún no ha comenzado.
 *
 * @param delay Puntero a la estructura `delay_t` que se va a inicializar.
 * @param duration Duración del retardo en milisegundos.
 */
void delayInit(delay_t *delay, tick_t duration) {

	if (delay == NULL) return false;
    delay->duration = duration;
    delay->running = false;

}

/**
 * @brief Lee el estado del retardo no bloqueante.
 *
 * Esta función verifica si el retardo ha expirado. Si el retardo no ha comenzado
 * (campo `running` es `false`), inicia el retardo tomando la marca de tiempo actual.
 * Si el retardo ya está en curso, compara la marca de tiempo actual con la inicial
 * para determinar si el retardo ha expirado.
 *
 * @param delay Puntero a la estructura `delay_t` que representa el retardo.
 * @return `true` si el retardo ha expirado, `false` si aún está en curso.
 */
bool_t delayRead(delay_t *delay) {

	if (delay == NULL) return false;

    if (!delay->running) {

        delay->startTime = HAL_GetTick();
        delay->running = true;
        return false;

    } else {

        tick_t currentTime = HAL_GetTick();
        if (currentTime - delay->startTime >= delay->duration) {

            delay->running = false;
            return true;

        } else {

            return false;

        }
    }
}

/**
 * @brief Modifica la duración de un retardo no bloqueante existente.
 *
 * Esta función permite cambiar la duración de un retardo no bloqueante ya inicializado.
 * La próxima vez que se lea el retardo (usando `delayRead`), se utilizará la nueva duración.
 *
 * @param delay Puntero a la estructura `delay_t` que representa el retardo.
 * @param duration Nueva duración del retardo en milisegundos.
 */
void delayWrite(delay_t *delay, tick_t duration) {

	if (delay == NULL) return false;
    delay->duration = duration;

}

/**
 *
 * PUNTO 3
 *
 * @brief Verifica si el retardo no bloqueante está actualmente corriendo.
 *
 * Esta función devuelve el estado del campo `running` de la estructura `delay_t`,
 * indicando si el retardo no bloqueante está en curso o no.
 *
 * @param delay Puntero a la estructura `delay_t` que representa el retardo.
 * @return `true` si el retardo está corriendo, `false` si no lo está.
 *
 */
bool_t delayIsRunning(delay_t *delay) {

	if (delay == NULL) return false;
    return delay->running;

}
