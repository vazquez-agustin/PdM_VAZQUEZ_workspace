/*
 * API_display.c
 *
 *  Created on: Jul 29, 2024
 *      Author: Agustin Vazquez
 */

/* Includes ------------------------------------------------------------------*/

#include "API_display.h"
#include "API_display_HAL.h"

typedef enum {
	false = 0,
	true,
} bool_t;

/* Private define ------------------------------------------------------------*/

#define CLEAR_DISPLAY             0x01
#define NIBBLE_MODE               0x28  // Configure LCD in 4 bits, 2 lines, 5x7 points
#define EIGHT_BITS_MODE           0x30  // Configure LCD in 8 bits
#define FOUR_BITS_MODE            0x20  // Configure LCD in 4 bits
#define DISPLAY_OFF               0x08  // Turn display off
#define CURSOR_RIGHT              0x06  // Move the cursor to the right
#define DISPLAY_WITHOUT_CURSOR    0x0C  // Turn display on, without cursor

// Definition of specific bits
#define LCD_ENABLE                0x04
#define RS_DATA                   0x01
#define RS_COMMAND                0x00
#define LCD_RW                    0x02
#define MASK_NIBBLE               0xF0
#define LINE1_ADDR                0x00
#define LINE2_ADDR                0x40
#define BACK_LIGHT                0x08  // Backlight control bit

// Delays
#define INITIAL_DELAY 50
#define DELAY5MS      5
#define DELAY2MS      2
#define DELAY1MS      1


/* Private function prototypes -----------------------------------------------*/

static void API_display_writeByte(uint8_t data, bool_t rs);
static void API_display_writeNibble(uint8_t nibble, bool_t rs);

/* Public API code -----------------------------------------------------------*/

/* Function Implementations --------------------------------------------------*/

/**
  * @brief  Initialize the LCD display.
  * @retval None
  */
void API_display_Init(void) {

	// Wait for 50 ms after power-up
	API_display_HAL_Delay(INITIAL_DELAY);

	// Initialize the display in 4-bit mode
	API_display_writeNibble(EIGHT_BITS_MODE, false); // Starts in 8-bit mode
	API_display_HAL_Delay(DELAY5MS);

	API_display_writeNibble(EIGHT_BITS_MODE, false);
	API_display_HAL_Delay(DELAY1MS);

	API_display_writeNibble(EIGHT_BITS_MODE, false);
	API_display_HAL_Delay(DELAY1MS);

	API_display_writeNibble(FOUR_BITS_MODE, false); // Changes to 4-bit mode
	API_display_HAL_Delay(DELAY1MS);

	// Function Set: 4-bit mode, 2 lines
	API_display_writeByte(NIBBLE_MODE, false); // Function set
	API_display_HAL_Delay(DELAY1MS);

	// Display ON/OFF Control: display ON, cursor OFF, blink OFF
	API_display_writeByte(DISPLAY_WITHOUT_CURSOR, false);
	API_display_HAL_Delay(DELAY1MS);

	// Clear Display
	API_display_writeByte(CLEAR_DISPLAY, false);
	API_display_HAL_Delay(DELAY2MS);

	// Entry Mode Set: increment mode, no shift
	API_display_writeByte(CURSOR_RIGHT, false);
	API_display_HAL_Delay(DELAY1MS);

}

/**
  * @brief  Clear the LCD display.
  * @retval None
  */
void API_display_Clear(void) {

	// Send the clear display command (0x01) in command mode (RS = 0).
	API_display_writeByte(CLEAR_DISPLAY, RS_COMMAND);
	// Introduce a delay of 1 ms to allow the display to process the clear operation.
	API_display_HAL_Delay(DELAY1MS);

}

/**
  * @brief  Send a character to the LCD display.
  * @param  character: Character to display.
  * @retval None
  */
void API_display_sendChar(uint8_t character) {

	API_display_writeByte(character, true);

}

/**
  * @brief  Send a string to the LCD display.
  * @param  str: Pointer to the string to display.
  * @retval None
  */
void API_display_sendString(char *str) {

	while (*str) { // Continue until the null terminator is reached.

		API_display_sendChar(*str); // Send each character to the display.
		str++; // Move to the next character in the string.

	}

}

/**
  * @brief  Set the cursor position on the LCD display.
  * @param  row: Row number (0 or 1).
  * @param  col: Column number (0 to max column).
  * @retval None
  */
void API_display_setCursor(uint8_t row, uint8_t col) {

	uint8_t address;

	// Calculate the address based on the row and column
	switch (row) {

	case 0:
		address = LINE1_ADDR + col; // Row 0 starts at address 0x00
		break;

	case 1:
		address = LINE2_ADDR + col; // Row 1 starts at address 0x40
		break;

	default:
		address = LINE1_ADDR + col;
		break;

	}

	// Set the address bit (DB7) to indicate we are sending a command
	address = address | 0x80;

	// Send the command to set the cursor position
	API_display_writeByte(address, false); // IN this case, false indicates a command

}

/* Private API code ----------------------------------------------------------*/

/* Function Implementations --------------------------------------------------*/

/**
  * @brief  Send a byte of data to the display in two 4-bit nibbles.
  * @param  data: Byte to send.
  * @param  rs: Register select flag (true for data, false for command).
  * @retval None
  */
static void API_display_writeByte(uint8_t data, bool_t rs) {

	// It sends the Most Significant Nibble (MSN) to the display LCD.
	// `data & MASK_NIBBLE` ensures that MSN is sent.
	API_display_writeNibble(data & MASK_NIBBLE, rs);
	API_display_HAL_Delay(DELAY1MS);

	// It sends the Less Significant Nibble (LSN) to the display LCD.
	// `(data << 4) & 0xF0` ensures that LSN is sent.
	API_display_writeNibble((data << 4) & MASK_NIBBLE, rs);
	API_display_HAL_Delay(DELAY1MS);
}

/**
  * @brief  Send a 4-bit nibble to the display.
  * @param  nibble: 4-bit data to send.
  * @param  rs: Register select flag (true for data, false for command).
  * @retval None
  */
static void API_display_writeNibble(uint8_t nibble, bool_t rs) {

	// Prepare the data byte by masking the lower 4 bits and keeping only the higher 4 bits.
	uint8_t data_u = (nibble & MASK_NIBBLE) | BACK_LIGHT; // with BACKLIGHT I set light in the screen

	// Set the RS (Register Select) bit according to the rs flag.
	if (rs == true) {
		data_u = data_u | RS_DATA;
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
