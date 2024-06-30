Trabajo Práctico N°1

Punto 1:

Implementar un programa que genere una secuencia periódica con los leds de la placa NUCLEO-F429ZI.

La secuencia debe seguir el orden: LED1, LED2, LED3, LED1,... etc.
Cada led debe permanecer encendido 200 ms.  No debe encenderse más de un led simultáneamente. Debe haber 200 ms de separación temporal entre encendidas sucesivas.

Ejemplo: Led1(ON) 200ms, Leds(OFF) 200ms, Led2(ON) 200ms, Leds(OFF) 200ms, Led3(ON) 200ms, Leds(OFF) 200ms, Led1(ON) 200ms, Leds(OFF) 200ms, etc...


Punto 2:

Utilizar el pulsador azul (USER_BUTTON) para controlar cómo se recorre la secuencia de leds.  Cada vez que se presiona el pulsador se debe alternar el orden de la secuencia entre:

LED1, LED2, LED3, LED1,... etc.
LED1, LED3, LED2, LED1,... etc.

Algunas preguntas para pensar el ejercicio:

* ¿De qué serviría usar un array de “leds” con la secuencia en el programa? ¿Qué pasa con nuestro programa si nos piden agregar/sacar/cambiar un led de la secuencia?

- Usar un arreglo de array para manejar los LEDs ayudaría a mejorar la escalabilidad y el código sería más fácil de mantener, ahorrando también más cantidad de líneas. Si luego quisiera agregar, eliminar o cambiar un LED de la secuencia, solo con modificar el array es suficiente. Esto evitaría el trabajo de cambiar múltiples líneas de código en el bucle principal.

* ¿Cómo responde el programa a las pulsaciones, hay falsos positivos o pulsaciones no detectadas? ¿Se implementa alguna técnica de antirrebote de las pulsaciones?

- El programa responde a las pulsaciones del botón implementando una técnica de antirrebote en la función ReadButton(). Aquí está cómo funciona:
Antirrebote: Utiliza una variable estática lastButtonState para almacenar el último estado conocido del botón y lastDebounceTime para registrar el momento en que se detectó un cambio de estado. Se aplica un tiempo de debounce de 50 ms (debounceDelay) usando HAL_GetTick() para medir el tiempo transcurrido desde el último cambio. Esto asegura que solo se considere una pulsación válida después de que haya pasado el tiempo de debounce sin cambios adicionales en el estado del botón.

- Falsos positivos: Esta técnica reduce la posibilidad de falsos positivos al asegurar que el estado del botón se mantenga estable durante el tiempo de debounce antes de considerar una pulsación como válida. Sin embargo, la efectividad depende de la mecánica y características eléctricas del botón físico.

* ¿Cuál es el mejor momento para leer el pulsador, luego de un ciclo completo de la secuencia o después de encender y apagar un led? ¿Qué diferencia hay entre estas alternativas?

- El mejor momento para leer el pulsador depende de los requisitos específicos del proyecto:
Después de un ciclo completo de la secuencia: Esto implica esperar a que se complete la secuencia actual antes de verificar el estado del botón. La ventaja es que el usuario puede cambiar la secuencia mientras no se está ejecutando ninguna acción visible en los LEDs.

- Después de encender y apagar un LED: Esto significa leer el estado del botón inmediatamente después de cambiar un LED de estado. La ventaja es que la respuesta del botón es más inmediata y el cambio en la secuencia se puede ver rápidamente.

* ¿Se puede cambiar el sentido de la secuencia en cualquier momento de la ejecución?
- Sí, el sentido de la secuencia se puede cambiar en cualquier momento de la ejecución. Esto se logra alternando la variable sequence cada vez que se detecta una pulsación válida del botón.

* ¿Cambiaría las respuestas a las preguntas anteriores si el tiempo de encendido de cada led fuera sensiblemente más grande, 1 segundo por ejemplo? ¿Y si fuera  sensiblemente más chico, 50 ms por ejemplo?

- Tiempo de encendido largo (1 segundo): Si el tiempo de encendido de cada LED es mucho mayor (1 segundo en este caso), el programa pasará más tiempo en cada ciclo de LED. Esto significa que el tiempo para detectar una pulsación válida del botón puede ser más largo, afectando la respuesta del sistema a las pulsaciones.

- Tiempo de encendido corto (50 ms): Con tiempos de encendido más cortos, el programa cambia más rápidamente de LED en cada ciclo. Esto puede hacer que sea más sensible a las pulsaciones del botón y potencialmente más difícil de interactuar si el debounce no es lo suficientemente robusto.
