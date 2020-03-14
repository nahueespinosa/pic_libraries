/*============================================================================
 * Licencia:
 * Autor:       Nahuel Espinosa
 * Fecha:       20/10/2019
 *===========================================================================*/
/** @file
 * @brief	Contiene las definiciones para el control de LCD por I2C
 * 
 * Aqui se encuentra la implementación de funciones, los defines, macros, 
 * datos internos y externos, declaraciones y definiciones de funciones
 * internas y definiciones de funciones externas
 */

/*==================[inclusiones]============================================*/
#include    <xc.h>
#include    "lcd_i2c.h"
#include    "mssp.h"
#define     _XTAL_FREQ          (4000000L)

/*==================[definiciones y macros]==================================*/
// commands
#define LCD_CLEARDISPLAY   0x01
#define LCD_CLRSCR         0x01
#define LCD_HOME           0x02
#define LCD_ENTRYMODESET   0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT    0x10
#define LCD_FUNCTIONSET    0x20
#define LCD_SETCGRAMADDR   0x40
#define LCD_SETDDRAMADDR   0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON      0x04
#define LCD_DISPLAYOFF     0x00
#define LCD_CURSORON       0x02
#define LCD_CURSOROFF      0x00
#define LCD_BLINKON        0x01
#define LCD_BLINKOFF       0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE    0x08
#define LCD_CURSORMOVE     0x00
#define LCD_MOVERIGHT      0x04
#define LCD_MOVELEFT       0x00

// flags for function set
#define LCD_8BITMODE       0x10
#define LCD_4BITMODE       0x00
#define LCD_2LINE          0x08
#define LCD_1LINE          0x00
#define LCD_5x10DOTS       0x04
#define LCD_5x8DOTS        0x00

// flags for backlight control
#define LCD_BACKLIGHT      0x00
#define LCD_NOBACKLIGHT    0x80

#define LCD_I2C_ADDR  (0x4E)

// Line addresses for LCDs which use
// the Hitachi HD44780U controller chip
#define LCD_LINE_1_ADDRESS 0x00
#define LCD_LINE_2_ADDRESS 0x40
#define LCD_LINE_3_ADDRESS 0x14
#define LCD_LINE_4_ADDRESS 0x54

typedef union {
    struct {    
        unsigned p_RS : 1;
        unsigned p_RW : 1;
        unsigned p_E  : 1;
        unsigned p_BL : 1;
        unsigned p_D4 : 1;
        unsigned p_D5 : 1;
        unsigned p_D6 : 1;
        unsigned p_D7 : 1;
    };
    uint8_t byte;
} lcd_pins_t;

/*==================[definiciones de datos internos]=========================*/
lcd_pins_t lcd_pins;

/*==================[definiciones de funciones externas]=====================*/
void lcd_config()
{
    i2cConfig();
    
    lcd_pins.p_BL = 1;
    
    __delay_ms(50);
    lcd_write_byte(0, LCD_HOME);
    __delay_ms(5);
    lcd_write_byte(0, LCD_FUNCTIONSET | LCD_2LINE | LCD_DISPLAYON );
    __delay_ms(5);
    lcd_write_byte(0, LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF );
    __delay_ms(5);
    lcd_write_byte(0, LCD_CLEARDISPLAY);
    __delay_ms(5);
    lcd_write_byte(0, LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT );
}

void lcd_backlight(uint8_t status)
{
    lcd_pins.p_BL = status;
    lcd_write_byte(0, 0x00);
}

void lcd_goto(uint8_t x, uint8_t y)   //x=col, y=row
{
    uint8_t address;

   switch(y)
     {
      case 1:
        address = LCD_LINE_1_ADDRESS;
        break;

      case 2:
        address = LCD_LINE_2_ADDRESS;
        break;

      case 3:
        address = LCD_LINE_3_ADDRESS;
        break;

      case 4:
        address = LCD_LINE_4_ADDRESS;
        break;

      default:
        address = LCD_LINE_1_ADDRESS;
        break;
     }

   address += x-1;
   lcd_write_byte(0, LCD_SETDDRAMADDR | address);
}

void lcd_write_string(const char *str)
{
   while (*str)
   {
        lcd_write_byte(1, *str); 
        str++;
   }
}

void lcd_write_byte(uint8_t address, uint8_t n)
{
    lcd_pins.p_RS  = address;
    lcd_pins.p_RW  = 0;
    lcd_pins.p_E   = 0;
    
    // Send upper nibble
    lcd_pins.byte &= 0x0F;
    lcd_pins.byte |= (n & 0xF0); 

    i2cStart();
    i2cWriteByte(LCD_I2C_ADDR+I2C_MODE_WRITE);
    lcd_pins.p_E = 0;
    i2cWriteByte(lcd_pins.byte);  //i2c_write(LCD_I2C_ADDR)
    lcd_pins.p_E = 1;
    i2cWriteByte(lcd_pins.byte);
    lcd_pins.p_E = 0;
    i2cWriteByte(lcd_pins.byte);
    i2cStop();
   
    // Send lower nibble
    lcd_pins.byte &= 0x0F;
    lcd_pins.byte |= (n << 4) & 0xF0; 

    i2cStart();
    i2cWriteByte(LCD_I2C_ADDR+I2C_MODE_WRITE);
    lcd_pins.p_E = 0;
    i2cWriteByte(lcd_pins.byte);
    lcd_pins.p_E = 1;
    i2cWriteByte(lcd_pins.byte);
    lcd_pins.p_E = 0;
    i2cWriteByte(lcd_pins.byte);
    i2cStop();
}

void lcd_clear()
{
    lcd_write_byte(0, LCD_CLRSCR);
    __delay_ms(5);
}

void lcd_clear_line(uint8_t line)
{
    lcd_goto(1,line);
    for (int i = 0; i<20; i++)
    {
        lcd_write_string(" ");
    }
    lcd_goto(1,line);
}

/*==================[fin del archivo]========================================*/