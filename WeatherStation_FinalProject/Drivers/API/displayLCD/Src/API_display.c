/*
 * API_display.c
 *
 *  Created on: Jul 29, 2024
 *      Author: ubuntu
 */


/* Includes ------------------------------------------------------------------*/

#include "API_display.h"
#include "API_display_HAL.h"

/* Private define ------------------------------------------------------------*/

#define CLEARdISPLAY 0x01 // Comando para limpiar la pantalla
#define NIBBLEmODE 0x28   // Configurar LCD en 4 bits, 2 líneas, 5x7 puntos
#define EIGHTbITSmODE 0x30 // Configurar LCD en 8 bits
#define FOURbITSmODE 0x20 // Configurar LCD en 4 bits
#define DISPLAYoFF 0x08  // Apaga el display
#define CURSORrIGHT 0x06  // Mueve el cursor a la derecha por cada escritura
#define DISPLAYwITHOUTcURSOR 0x0C  //Enciende el display, sin cursor

#define LINE1aDDR 0x00
#define LINE2aDDR 0x40

// Dirección I2C del expansor de I/O PCF8574
#define LCD_ADDR 0x27
#define LCD_CMD 0
#define LCD_DATA 1



/* Function Implementations --------------------------------------------------*/

// Envía un comando al LCD
void LCD_SendCommand(I2C_HandleTypeDef *hi2c, uint8_t command) {
    uint8_t buf[2] = {0x00, command};
    API_display_HAL_I2C_Write(hi2c, LCD_ADDR << 1, buf, 2);
}

// Envía datos al LCD
void LCD_SendData(I2C_HandleTypeDef *hi2c, uint8_t data) {
    uint8_t buf[2] = {0x40, data};
    API_display_HAL_I2C_Write(hi2c, LCD_ADDR << 1, buf, 2);
}

// Inicializa el LCD
void API_display_Init(I2C_HandleTypeDef *hi2c) {

    API_display_HAL_Delay(50);  // Esperar a que el LCD se inicialice
    LCD_SendCommand(hi2c, EIGHTbITSmODE);
    API_display_HAL_Delay(5);
    LCD_SendCommand(hi2c, EIGHTbITSmODE);
    API_display_HAL_Delay(1);
    LCD_SendCommand(hi2c, EIGHTbITSmODE);

    LCD_SendCommand(hi2c, FOURbITSmODE);  // Usar modo de 4 bits

    LCD_SendCommand(hi2c, NIBBLEmODE);  // Configurar LCD en 4 bits, 2 líneas, 5x7 puntos

    LCD_SendCommand(hi2c, DISPLAYoFF);  // Apagar display

    LCD_SendCommand(hi2c, CLEARdISPLAY);  // Limpiar display
    API_display_HAL_Delay(2);

    LCD_SendCommand(hi2c, CURSORrIGHT);  // Mover cursor a la derecha

    LCD_SendCommand(hi2c, DISPLAYwITHOUTcURSOR);  // Encender display, sin cursor

}

// Limpia el LCD
void API_display_Clear(I2C_HandleTypeDef *hi2c) {

    LCD_SendCommand(hi2c, CLEARdISPLAY);  // Comando para limpiar la pantalla
    API_display_HAL_Delay(2);  // Esperar a que el LCD procese el comando

}

// Configura el cursor del LCD en una posición específica
void API_display_Set_Cursor(I2C_HandleTypeDef *hi2c, uint8_t row, uint8_t col) {

    uint8_t address;

    // Establecer la dirección de la memoria de datos (DDR) basada en la fila y la columna
    switch(row) {
        case 0:
            address = LINE1aDDR + col;
            break;
        case 1:
            address = LINE2aDDR + col;
            break;
        default:
            address = LINE1aDDR + col;
            break;
    }

    // Enviar comando para establecer la posición del cursor
    LCD_SendCommand(hi2c, 0x80 | address);
}

// Imprime una cadena de caracteres en el LCD
void API_display_Print(I2C_HandleTypeDef *hi2c, const char *str) {
    while (*str) {
        LCD_SendData(hi2c, *str++);
    }
}
