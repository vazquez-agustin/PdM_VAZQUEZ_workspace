### FINITE STATE MACHINE


stateDiagram-v2
    [*] --> STATE_IDLE

    STATE_IDLE : Init system: sensor, display, etc.
    STATE_READ_SENSOR : Read temperature from sensor
    STATE_CONTROL : Check temperature within limits
    STATE_ALARM : Alarm triggered, activate relay

    STATE_IDLE --> STATE_READ_SENSOR : delayRead() complete
    STATE_READ_SENSOR --> STATE_CONTROL : Temperature read
    STATE_CONTROL --> STATE_IDLE : Temperature OK
    STATE_CONTROL --> STATE_ALARM : Temperature exceeds threshold
    STATE_ALARM --> STATE_IDLE : Temperature back to normal after delay
    STATE_ALARM --> STATE_READ_SENSOR : delayRead() complete

