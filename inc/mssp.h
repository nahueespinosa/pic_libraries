/*============================================================================
 * Licencia:
 * Autor:       Leonardo Davico
 * Fecha:       22/09/2018
 *===========================================================================*/
/** @file
 * @brief	Contiene macros y definiciones del puerto serie sincronico
 * 
 * Aqui se definen macros, tipos de datos y declaración de funciones y datos 
 * externos deteclado
 */
#ifndef MSSP_H
#define MSSP_H

/*==================[inclusiones]============================================*/
#include <stdint.h>     /* para las definiciones de uint8_t por ej.*/

/*==================[macros]=================================================*/
//Pines del Puerto MSSP
#define	TRIS_SCK    TRISCbits.TRISC3
#define	PIN_SCK     PORTCbits.RC3
#define	TRIS_SDI    TRISCbits.TRISC4
#define	PIN_SDI     PORTCbits.RC4
#define	TRIS_SDO    TRISCbits.TRISC5
#define	PIN_SDO     PORTCbits.RC5

#define	I2C_FCLOCK   (100000L)       //Frecuencia de clock para I2C en Hz
#define _XTAL_FREQ   (4000000L)

/*==================[tipos de datos declarados por el usuario]===============*/
typedef enum {
    SPI_MASTER_OSC_DIV4  = 0b00100000,
    SPI_MASTER_OSC_DIV16 = 0b00100001,
    SPI_MASTER_OSC_DIV64 = 0b00100010,
    SPI_MASTER_TMR2      = 0b00100011,
    SPI_SLAVE_SS_EN      = 0b00100100,
    SPI_SLAVE_SS_DIS     = 0b00100101
}spiType_t;

typedef enum {
    SPI_DATA_SAMPLE_MIDDLE = 0b00000000,
    SPI_DATA_SAMPLE_END    = 0b10000000
}spiDataSample_t;

typedef enum {
    SPI_CLOCK_IDLE_HIGH  = 0b00010000,
    SPI_CLOCK_IDLE_LOW   = 0b00000000        
}spiClockIdle_t;

typedef enum {
    SPI_IDLE_2_ACTIVE    = 0b00000000,
    SPI_ACTIVE_2_IDLE    = 0b01000000
}spiTransmitEdge_t;

typedef enum {
    I2C_MODE_WRITE = 0x00,
    I2C_MODE_READ  = 0x01
}i2cModeRW_t;

/*==================[declaraciones de datos externos]========================*/

/*==================[declaraciones de funciones externas]====================*/
/*****************************************************************************/
/*  Funciones SPI                                                            */
/*****************************************************************************/
void spiConfig( spiType_t, spiDataSample_t, spiClockIdle_t, spiTransmitEdge_t);
void spiWriteByte(uint8_t value);
bit spiDataReady(void);
uint8_t spiReadByte(void);

/*****************************************************************************/
/*  Funciones I2C                                                            */
/*****************************************************************************/
void i2cConfig(void);
void i2cWait(void);
void i2cStart(void);
void i2cReStart(void);
void i2cStop(void);
bit i2cWriteByte(uint8_t value);
uint8_t i2cReadByte(void);
void i2cReadAck(void);
void i2cReadNoAck(void);

/*==================[fin del archivo]========================================*/
#endif// MSSP_H
