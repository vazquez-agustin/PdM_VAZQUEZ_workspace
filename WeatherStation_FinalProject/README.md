### Trabajo Práctico Final: Estación de Control de Temperatura

## Programación de Microcontroladores
## Protocolos de Comunicación en Sistemas Embebidos

* Alumno: Ing. Agustín Jesús Vazquez

* Profesor PdM: Mg. Ing. Patricio Bos
* Profesor PCSE: Ing. Israel Pavelek

# Descripción del proyecto

La aplicación propuesta consiste en la medición de los parámetros de temperatura, presión y humedad, a través del sensor BME680. 
Los resultados de dichas mediciones se visualizan en un display LCD 1602.


## FINITE STATE MACHINE

```mermaid
stateDiagram-v2
[*] --> SI
state "STATLE IDLE" AS SI: Waiting a timeout
state "STATE READ SENSOR" as SRS: Reading temperatura
state "STATE CONTROL" as SC: Check temperature within limits
state "STATE ALARM" as SA: Alarm triggered, activate relay
SI --> SRS : Delay timer expired
SRS --> SC : Temperature read
SC --> SI  : Temperature OK
SC --> SA  : Temperature exceeds threshold
SA --> SI  : Temperature back to normal after delay
SA --> SA  : Temperature read
```
