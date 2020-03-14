/*============================================================================
 * Licencia:
 * Autor:       Nahuel Espinosa
 * Fecha:       20/10/2019
 *===========================================================================*/
/** @file
 * @brief	Contiene macros y definiciones para el control de LCD por I2C
 * 
 * Aqui se definen macros, tipos de datos y declaración de funciones
 */
#ifndef _LCD_I2C_H_
#define _LCD_I2C_H_

/*==================[inclusiones]============================================*/
#include <stdint.h>

/*==================[macros]=================================================*/

/*==================[declaraciones de funciones externas]====================*/
void lcd_backlight(uint8_t status);
void lcd_clear_line(uint8_t line);
void lcd_clear();
void lcd_goto(uint8_t x, uint8_t y);
void lcd_config( void );;
void lcd_write_byte(uint8_t address, uint8_t n);
void lcd_write_string(const char *str);

/*==================[fin del archivo]========================================*/
#endif /* _LCD_I2C_H */