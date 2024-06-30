Trabajo Práctico N°1

Punto 1:

Implementar un programa que genere una secuencia periódica con los leds de la placa NUCLEO-F429ZI.

La secuencia debe seguir el orden: LED1, LED2, LED3, LED1,... etc.
Cada led debe permanecer encendido 200 ms.  No debe encenderse más de un led simultáneamente. Debe haber 200 ms de separación temporal entre encendidas sucesivas.

Ejemplo: Led1(ON) 200ms, Leds(OFF) 200ms, Led2(ON) 200ms, Leds(OFF) 200ms, Led3(ON) 200ms, Leds(OFF) 200ms, Led1(ON) 200ms, Leds(OFF) 200ms, etc...

Algunas preguntas para pensar el ejercicio:

* ¿De qué serviría usar un array de “leds” con la secuencia en el programa? ¿Qué pasa con nuestro programa si nos piden agregar/sacar/cambiar un led de la secuencia?

Usar un arreglo de array para manejar los LEDs ayudaría a mejorar la escalabilidad y el código sería más fácil de mantener, ahorrando también más cantidad de líneas. Si luego quisiera agregar, eliminar o cambiar un LED de la secuencia, solo con modificar el array es suficiente. Esto evitaría el trabajo de cambiar múltiples líneas de código en el bucle principal.
