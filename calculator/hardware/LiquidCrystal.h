#ifndef LiquidCrystal_h
#define LiquidCrystal_h

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// Register Definitions
#define LCD_CTRL_PORT PORTB
#define LCD_CTRL_DDR  DDRB
#define LCD_DATA_PORT PORTD
#define LCD_DATA_DDR  DDRD

// Pin Definitions
#define LCD_RS        PB0
#define LCD_EN        PB1

// RW is usually grounded (not used) to save pins. 
#define LCD_D4        PD4
#define LCD_D5        PD5
#define LCD_D6        PD6
#define LCD_D7        PD7

// Commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME   0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_FUNCTIONSET  0x20
#define LCD_SETDDRAMADDR 0x80

// Flags
#define LCD_DISPLAYON    0x04
#define LCD_DISPLAYOFF   0x00
#define LCD_CURSOROFF    0x00
#define LCD_BLINKOFF     0x00
#define LCD_ENTRYLEFT    0x02
#define LCD_ENTRYSHIFTDECREMENT 0x00
#define LCD_4BITMODE     0x00
#define LCD_2LINE        0x08
#define LCD_5x8DOTS      0x00

// Function Prototypes
void lcd_init(void);
void lcd_clear(void);
void lcd_print_string(char *str);

// Low level (usually not needed by user, but available)
void lcd_command(uint8_t value);
void lcd_write(uint8_t value);

#endif

//This part was generated because it is cumbersome to replace every arduino function in liquidcrystal.h manually
