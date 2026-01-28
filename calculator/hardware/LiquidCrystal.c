#include "LiquidCrystal.h"
#include <string.h>

// Internal Helper Prototypes
static void lcd_send(uint8_t value, uint8_t mode);
static void lcd_write4bits(uint8_t value);
static void lcd_pulseEnable(void);

// Helper Macros for Pin Manipulation
#define RS_LOW   LCD_CTRL_PORT &= ~(1 << LCD_RS)
#define RS_HIGH  LCD_CTRL_PORT |= (1 << LCD_RS)
#define EN_LOW   LCD_CTRL_PORT &= ~(1 << LCD_EN)
#define EN_HIGH  LCD_CTRL_PORT |= (1 << LCD_EN)

// Globals to track display state (kept internal to this file)
static uint8_t _displayfunction;
static uint8_t _displaycontrol;
static uint8_t _displaymode;

void lcd_init(void) {
    // 1. Set Data Direction Registers (DDR) to OUTPUT
    LCD_CTRL_DDR |= (1 << LCD_RS) | (1 << LCD_EN);
    LCD_DATA_DDR |= (1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7);

    // Ensure EN and RS are low
    RS_LOW;
    EN_LOW;

    // 2. Initial Startup Delay (Wait > 40ms after power up)
    _delay_ms(50); 

    // 3. Initialization Sequence (4-bit specific magic sequence)
    // We start in 8-bit mode by default, try to set 4-bit mode
    
    // First try
    lcd_write4bits(0x03);
    _delay_us(4500); // wait min 4.1ms

    // Second try
    lcd_write4bits(0x03);
    _delay_us(4500); // wait min 4.1ms

    // Third try
    lcd_write4bits(0x03); 
    _delay_us(150);

    // Finally, set to 4-bit interface
    lcd_write4bits(0x02); 

    // 4. Set Function: 4-bit, 2 lines, 5x8 dots
    _displayfunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;
    lcd_command(LCD_FUNCTIONSET | _displayfunction);

    // 5. Turn Display On (Cursor/Blink Off)
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);

    // 6. Clear Display
    lcd_clear();

    // 7. Set Entry Mode (Left to Right)
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    lcd_command(LCD_ENTRYMODESET | _displaymode);
}

void lcd_clear(void) {
    lcd_command(LCD_CLEARDISPLAY);
    _delay_ms(2); // Clear command takes a long time
}

// Optimized to print a string and wrap to line 2 automatically
void lcd_print_string(char *str) {
    uint8_t count = 0;
    
    while (*str) {
        lcd_write((uint8_t)*str);
        count++;
        str++;

        // Logic to handle 16x2 wrapping
        // If we hit the 16th character, move cursor to the second line
        if (count == 16) {
            lcd_command(LCD_SETDDRAMADDR | 0x40); // 0x40 is the address of Line 2
        }
        // Safety: If string is longer than 32 chars, we stop or loop. 
        // For now, we just let it write to hidden memory.
    }
}

// Low level command sender
void lcd_command(uint8_t value) {
    lcd_send(value, 0); // 0 = LOW (Command)
}

// Low level data sender
void lcd_write(uint8_t value) {
    lcd_send(value, 1); // 1 = HIGH (Data)
}

// Core function to push data/commands
static void lcd_send(uint8_t value, uint8_t mode) {
    if (mode) {
        RS_HIGH;
    } else {
        RS_LOW;
    }

    // Write high nibble
    lcd_write4bits(value >> 4);
    // Write low nibble
    lcd_write4bits(value);
}

// Push 4 bits to the hardware pins
static void lcd_write4bits(uint8_t value) {
    // Clear the data pins first
    LCD_DATA_PORT &= ~((1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7));

    // Set pins based on the lower 4 bits of 'value'
    if ((value >> 0) & 0x01) LCD_DATA_PORT |= (1 << LCD_D4);
    if ((value >> 1) & 0x01) LCD_DATA_PORT |= (1 << LCD_D5);
    if ((value >> 2) & 0x01) LCD_DATA_PORT |= (1 << LCD_D6);
    if ((value >> 3) & 0x01) LCD_DATA_PORT |= (1 << LCD_D7);

    lcd_pulseEnable();
}

static void lcd_pulseEnable(void) {
    EN_LOW;
    _delay_us(1);    
    EN_HIGH;
    _delay_us(1);    // Enable pulse must be >450 ns
    EN_LOW;
    _delay_us(100);  // Commands need >37 us to settle
}

//this part was generated because it is cumbersome to replace every arduino function in liquidcrystal.h manually
