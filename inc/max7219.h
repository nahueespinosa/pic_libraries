/*============================================================================
 * Licencia:
 * Autor:       Nahuel Espinosa
 * Fecha:       31/07/2018
 *===========================================================================*/
/** @file
 * @brief	Contiene macros y definiciones para el MAX7219
 * 
 * Aqui se definen macros, tipos de datos y declaración de funciones
 */
#ifndef MAX7219_H
#define MAX7219_H

/*==================[inclusiones]============================================*/
#include <xc.h> 
#include <stdint.h>

/*==================[macros]=================================================*/
#define _XTAL_FREQ  4000000L

#define TRIS_SDO    TRISCbits.TRISC5
#define TRIS_SDI    TRISCbits.TRISC4
#define TRIS_SCK    TRISCbits.TRISC3

#define TRIS_SS1    TRISCbits.TRISC0
#define TRIS_SS2    TRISCbits.TRISC1
#define TRIS_SS3    TRISDbits.TRISD2
#define TRIS_SS4    TRISDbits.TRISD3
	
#define PIN_SS1	    PORTCbits.RC0
#define PIN_SS2	    PORTCbits.RC1
#define PIN_SS3	    PORTDbits.RD2
#define PIN_SS4	    PORTDbits.RD3

#define REG_NO_OP           0x00
#define REG_DIGIT_0         0x01
#define REG_DIGIT_1         0x02
#define REG_DIGIT_2         0x03
#define REG_DIGIT_3         0x04
#define REG_DIGIT_4         0x05
#define REG_DIGIT_5         0x06
#define REG_DIGIT_6         0x07
#define REG_DIGIT_7         0x08
#define REG_DECODE_MODE     0x09
#define REG_INTENSITY       0x0A
#define REG_SCAN_LIMIT      0x0B
#define REG_SHUTDOWN        0x0C
#define REG_DISPLAY_TEST    0x0F

/*==================[declaraciones de funciones externas]====================*/
void    max7219_config();
void    max7219_shutdown      (uint8_t addr, uint8_t state);
void    max7219_set_scan_limit(uint8_t addr, uint8_t limit);
void    max7219_set_intensity (uint8_t addr, uint8_t intensity);
void    max7219_clear_display (uint8_t addr);
void    max7219_set_led       (uint8_t addr, uint8_t row, uint8_t col, uint8_t state);
uint8_t max7219_get_led       (uint8_t addr, uint8_t row, uint8_t col);
void    max7219_set_row       (uint8_t addr, uint8_t row, uint8_t value);
uint8_t max7219_get_row       (uint8_t addr, uint8_t row);
void    max7219_set_column    (uint8_t addr, uint8_t col, uint8_t value);
uint8_t max7219_get_column    (uint8_t addr, uint8_t col);

/*==================[fin del archivo]========================================*/
#endif	/* MAX7219_H */