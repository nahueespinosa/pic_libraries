
 /*============================================================================
 * Licencia:
 * Autor:       Nahuel Espinosa
 * Fecha:       31/07/2018
 *===========================================================================*/
/** @file
 * @brief	Contiene las definiciones para el control del MAX7219
 * 
 * Aqui se encuentra la implementación de funciones, los defines, macros, 
 * datos internos y externos, declaraciones y definiciones de funciones
 * internas y definiciones de funciones externas
 */

/*==================[inclusiones]============================================*/
#include "max7219.h"

/*==================[definiciones de datos internos]=========================*/
uint8_t matrix[4][8];

/*==================[definiciones de funciones internas]=====================*/
static uint8_t max7219_write(uint8_t addr, uint8_t reg_addr, uint8_t data) {
    uint8_t ret_val = 0;
    
    switch (addr) {
        case 0:
            PIN_SS1 = 0;
            break;
        case 1:
            PIN_SS2 = 0;
            break;
        case 2:
            PIN_SS3 = 0;
            break;
        case 3:
            PIN_SS4 = 0;
            break;
        default:
            break;
    }

    
    SSPBUF = reg_addr;
    while (!SSPSTATbits.BF);
    ret_val = SSPBUF;
    
    SSPBUF = data;
    while (!SSPSTATbits.BF);
    ret_val = SSPBUF;

    switch (addr) {
        case 0:
            PIN_SS1 = 1;
            break;
        case 1:
            PIN_SS2 = 1;
            break;
        case 2:
            PIN_SS3 = 1;
            break;
        case 3:
            PIN_SS4 = 1;
            break;
        default:
            break;
    }

    return ret_val;
}

/*==================[definiciones de funciones externas]=====================*/
void max7219_config() {
    TRIS_SDO = 0;
    TRIS_SDI = 1;
    TRIS_SCK = 0;
    TRIS_SS1 = 0;
    TRIS_SS2 = 0;
    TRIS_SS3 = 0;
    TRIS_SS4 = 0;

    SSPCONbits.CKP = 0;
    SSPCONbits.SSPM = 0b0010;
    SSPCONbits.SSPEN = 1;

    SSPSTATbits.CKE = 1;

    PIN_SS1 = 1;
    PIN_SS2 = 1;
    PIN_SS3 = 1;
    PIN_SS4 = 1;

    max7219_write(0, REG_DECODE_MODE, 0x00);
    max7219_write(1, REG_DECODE_MODE, 0x00);
    max7219_write(2, REG_DECODE_MODE, 0x00);
    max7219_write(3, REG_DECODE_MODE, 0x00);
}

void max7219_shutdown(uint8_t addr, uint8_t b) {
    max7219_write(addr, REG_SHUTDOWN, !b);
}

void max7219_set_scan_limit(uint8_t addr, uint8_t limit) {
    max7219_write(addr, REG_SCAN_LIMIT, limit);
}

void max7219_set_intensity(uint8_t addr, uint8_t intensity) {
    max7219_write(addr, REG_INTENSITY, intensity);
}

void max7219_clear_display(uint8_t addr) {
    uint8_t index;

    for (index = 0; index < 8; index++) {
        max7219_write(addr, REG_DIGIT_0 + index, 0b00000000);
        matrix[addr][index] = 0b00000000;
    }
}

void max7219_set_led(uint8_t addr, uint8_t row, uint8_t col, uint8_t state) {
    if (state) {
        matrix[addr][row] |= (0x1 << col);
    } else {
        matrix[addr][row] &= ~(0x1 << col);
    }

    max7219_write(addr, REG_DIGIT_0 + row, matrix[addr][row]);
}

uint8_t max7219_get_led(uint8_t addr, uint8_t row, uint8_t col) {
    return (matrix[addr][row] >> col) & 0x01;
}

void max7219_set_row(uint8_t addr, uint8_t row, uint8_t value) { 
    matrix[addr][row] = value;
    max7219_write(addr, REG_DIGIT_0 + row, value);
}

uint8_t max7219_get_row(uint8_t addr, uint8_t row) {    
    return matrix[addr][row];
}

void max7219_set_column(uint8_t addr, uint8_t col, uint8_t value) {
    uint8_t index;
    for (index = 0; index < 8; index++) {
        max7219_set_led(addr, index, col, (value >> index) & 0x01);
    }
}

uint8_t max7219_get_column(uint8_t addr, uint8_t col) {
    uint8_t ret_val = 0x00, index;
    for (index = 0; index < 8; index++) {
        ret_val |= (max7219_get_led(addr, index, col) << index);
    }
    return ret_val;
}

/*==================[fin del archivo]========================================*/