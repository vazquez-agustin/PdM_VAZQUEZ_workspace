/*
 * API_display.c
 *
 *  Created on: Jul 29, 2024
 *      Author: ubuntu
 */

/* Includes ------------------------------------------------------------------*/

#include "API_display.h"
#include "API_display_HAL.h"

typedef enum {
	false = 0,
	true,
} bool_t;

/* Private define ------------------------------------------------------------*/

#define CLEARdISPLAY 0x01 // Comando para limpiar la pantalla
#define NIBBLEmODE 0x28   // Configurar LCD en 4 bits, 2 líneas, 5x7 puntos
#define EIGHTbITSmODE 0x30 // Configurar LCD en 8 bits
#define FOURbITSmODE 0x20 // Configurar LCD en 4 bits
#define DISPLAYoFF 0x08  // Apaga el display
#define CURSORrIGHT 0x06  // Mueve el cursor a la derecha por cada escritura
#define DISPLAYwITHOUTcURSOR 0x0C  //Enciende el display, sin cursor

// Definition of specific bits
#define LCD_ENABLE 0x04
#define RSdATA 0x01
#define RScOMMAND 0x00
#define LCD_RW 0x02
#define MASKnIBBLE 0xF0
#define LINE1aDDR 0x00
#define LINE2aDDR 0x40
#define BACKlIGHT 0x08 // Backlight control bit


/* Private function prototypes -----------------------------------------------*/

static void API_display_writeByte(uint8_t data, bool_t rs);
static void API_display_writeNibble(uint8_t nibble, bool_t rs);

/* Public API code -----------------------------------------------------------*/

/* Function Implementations --------------------------------------------------*/

/**
 * @brief Initializes the LCD display.
 *
 * This function initializes the LCD display by sending a sequence of commands to set up
 * the display in 4-bit mode, configure display settings, and clear the display.
 * It follows the initialization sequence specified for LCDs based on the HD44780 controller.
 */
void API_display_Init(void) {
	// Wait for 50 ms after power-up
	API_display_HAL_Delay(50);

	// Initialize the display in 4-bit mode
	API_display_writeNibble(0x30, false); // Command to set to 8-bit mode
	API_display_HAL_Delay(5);             // Wait for 5 ms

	API_display_writeNibble(0x30, false); // Command to set to 8-bit mode
	API_display_HAL_Delay(1);             // Wait for 1 ms

	API_display_writeNibble(0x30, false); // Command to set to 8-bit mode
	API_display_HAL_Delay(1);             // Wait for 1 ms

	API_display_writeNibble(0x20, false); // Command to set to 4-bit mode
	API_display_HAL_Delay(1);             // Wait for 1 ms

	// Function Set: 4-bit mode, 2 lines, 5x8 font
	API_display_writeByte(0x28, false); // Function set
	API_display_HAL_Delay(1);           // Wait for 1 ms

	// Display ON/OFF Control: display ON, cursor OFF, blink OFF
	API_display_writeByte(0x0C, false); // Display control
	API_display_HAL_Delay(1);           // Wait for 1 ms

	// Clear Display
	API_display_writeByte(0x01, false); // Clear display
	API_display_HAL_Delay(2);           // Wait for 2 ms

	// Entry Mode Set: increment mode, no shift
	API_display_writeByte(0x06, false); // Entry mode set
	API_display_HAL_Delay(1);           // Wait for 1 ms
}

/**
 * @brief Clears the LCD display.
 *
 * This function sends the clear display command to the LCD. It utilizes the `API_display_writeByte`
 * function to send the clear display command in command mode (RS = 0). After sending the command,
 * it introduces a delay to ensure the display has enough time to process the clear operation.
 */
void API_display_Clear(void) {
	// Send the clear display command (0x01) in command mode (RS = 0).
	API_display_writeByte(CLEARdISPLAY, RScOMMAND);
	// Introduce a delay of 1 ms to allow the display to process the clear operation.
	API_display_HAL_Delay(1);
}

/**
 * @brief Sends a character to the LCD display.
 *
 * This function sends a single character to the LCD display by utilizing the `API_display_writeByte` function.
 * It ensures that the character is sent in data mode by setting the RS (Register Select) flag to true.
 *
 * @param character The character to be sent to the display.
 */
void API_display_SendChar(uint8_t character) {
	API_display_writeByte(character, true);
}

/**
 * @brief Sends a string to the LCD display.
 *
 * This function sends a null-terminated string to the LCD display by utilizing the `API_display_SendChar` function.
 * It iterates through each character in the string and sends it to the display.
 *
 * @param str The null-terminated string to be sent to the display.
 */
void API_display_SendString(char *str) {
	while (*str) { // Continue until the null terminator is reached.
		API_display_SendChar(*str); // Send each character to the display.
		str++; // Move to the next character in the string.
	}
}

/**
 * @brief Sets the cursor position on the LCD display.
 *
 * This function sets the cursor to the specified row and column on the LCD display.
 * It calculates the appropriate address for the given position and sends the corresponding
 * command to the display to move the cursor.
 *
 * @param row The row position (0 or 1).
 * @param col The column position (0 to 15).
 */
void API_display_SetCursor(uint8_t row, uint8_t col) {
	uint8_t address;

	// Calculate the address based on the row and column
	switch (row) {
	case 0:
		address = LINE1aDDR + col; // Row 0 starts at address 0x00
		break;
	case 1:
		address = LINE2aDDR + col; // Row 1 starts at address 0x40
		break;
	default:
		// If an invalid row is provided, default to row 0
		address = LINE1aDDR + col;
		break;
	}

	// Set the address bit (DB7) to indicate we are sending a command
	address = address | 0x80;

	// Send the command to set the cursor position
	API_display_writeByte(address, false); // false indicates a command
}

/* Private API code ----------------------------------------------------------*/

/**
 * @brief Sends a byte to the LCD display in nibble format.
 *
 * This function splits the byte into two nibbles (high and low) and sends them to the LCD display
 * using the `API_display_writeNibble` function. The byte is sent in two steps: first the high nibble
 * and then the low nibble. A delay is introduced after each step to ensure that the LCD processes
 * the data correctly.
 *
 * @param data The byte of data to be sent to the display.
 * @param rs A flag indicating whether the data is a command (rs = 0) or data (rs = 1).
 */
static void API_display_writeByte(uint8_t data, bool_t rs) {

	// Enviar el nibble alto al display LCD.
	// `data & 0xF0` asegura que solo el nibble alto sea enviado.
	// El parámetro `rs` se usa para definir el registro de selección (RS) en el display.
	API_display_writeNibble(data & MASKnIBBLE, rs);
	API_display_HAL_Delay(1);

	// Enviar el nibble bajo al display LCD.
	// `(data << 4) & 0xF0` asegura que solo el nibble bajo sea enviado.
	// El parámetro `rs` se usa para definir el registro de selección (RS) en el display.
	API_display_writeNibble((data << 4) & MASKnIBBLE, rs);
	API_display_HAL_Delay(1);
}

/**
 * @brief Sends a nibble to the LCD display.
 *
 * This function takes a 4-bit nibble, combines it with control bits, and sends it to the LCD display
 * using the I2C protocol. The function ensures that the correct control bits are set for either
 * data or command mode and toggles the enable bit to signal the LCD to read the nibble.
 *
 * @param nibble The 4-bit nibble to send to the display (higher 4 bits should be used).
 * @param rs A flag indicating whether the nibble is a command (rs = false) or data (rs = true).
 */
static void API_display_writeNibble(uint8_t nibble, bool_t rs) {

	// Prepare the data byte by masking the lower 4 bits and keeping only the higher 4 bits.
	uint8_t data_u = (nibble & MASKnIBBLE) | BACKlIGHT; // with BACKLIGHT I set light in the screen

	// Set the RS (Register Select) bit according to the rs flag.
	if (rs == true) {
		data_u = data_u | RSdATA;
	}

	// Array to hold the two bytes that will be sent to the LCD.
	uint8_t data_t[2];

	// Set the enable bit (EN) high, along with RS and data bits.
	data_t[0] = data_u | LCD_ENABLE; // 0x04 sets the EN bit to high.

	// Set the enable bit (EN) low, along with RS and data bits.
	data_t[1] = data_u; // EN bit is cleared to low.

	// Transmit the prepared bytes to the LCD via I2C.
	API_display_HAL_I2C_Write(data_t, 2);

}
