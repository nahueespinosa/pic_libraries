/*============================================================================
 * Licencia:
 * Autor:       Nahuel Espinosa
 * Fecha:       20/10/2019
 *===========================================================================*/
/** @file
 * @brief	Contiene las definiciones para el módulo DS1307
 * 
 * Aqui se encuentra la implementación de funciones, los defines, macros, 
 * datos internos y externos, declaraciones y definiciones de funciones
 * internas y definiciones de funciones externas
 */

/*==================[inclusiones]============================================*/
#include <xc.h>
#include "ds1307.h"
#include "mssp.h"

/*==================[definiciones y macros]==================================*/
#define RTC_SECONDS_ADDR     (0x00)
#define RTC_MINUTES_ADDR     (0x01)
#define RTC_HOURS_ADDR       (0x02)
#define RTC_DAY_ADDR         (0x03)
#define RTC_DATE_ADDR        (0x04)
#define RTC_MONTH_ADDR       (0x05)
#define RTC_YEAR_ADDR        (0x06)
#define RTC_CONTROL_ADDR     (0x07)
#define RTC_RAM_ADDR         (0x08)

#define RTC_OUTPUT_OFF       (0x00)
#define RTC_OUTPUT_ON        (0x80)

#define RTC_SQW_DISABLED     (0x00)
#define RTC_SQW_ENABLED      (0x10)

#define RTC_SQW_RATE_1HZ     (0x00)
#define RTC_SQW_RATE_4096HZ  (0x01)
#define RTC_SQW_RATE_8192HZ  (0x02)
#define RTC_SQW_RATE_32768HZ (0x03)

#define RTC_I2C_ADDR         (0xD0)

/*==================[definiciones de funciones internas]=====================*/
uint8_t bcd_to_int( uint8_t byte ) {
    uint8_t d, u;
    
    d = (byte & 0xF0) >> 4;
    u = (byte & 0x0F);
    
    return (d * 10) + u;
}

uint8_t int_to_bcd( uint8_t byte ) {
    uint8_t d, u;
    
    d = (byte / 10);
    u = byte % 10;
    
    return (d << 4) + u;
}

/*==================[definiciones de funciones externas]=====================*/
void ds1307_config( void ) {
    i2cConfig();
    
    i2cStart();
    i2cWriteByte( RTC_I2C_ADDR | I2C_MODE_WRITE );
    i2cWriteByte( RTC_CONTROL_ADDR );
    i2cWriteByte( RTC_OUTPUT_OFF | RTC_SQW_DISABLED | RTC_SQW_RATE_1HZ );
    
    i2cStop();
}

void ds1307_set_date( uint8_t day, uint8_t month, uint8_t year ) {
    i2cStart();
    
    i2cWriteByte( RTC_I2C_ADDR | I2C_MODE_WRITE );
    i2cWriteByte( RTC_DATE_ADDR );
    
    i2cWriteByte( int_to_bcd(day) );
    i2cWriteByte( int_to_bcd(month) );
    i2cWriteByte( int_to_bcd(year) );
    
    i2cStop();
}

void ds1307_set_time( uint8_t hour, uint8_t min, uint8_t sec ) {
    i2cStart();
    
    i2cWriteByte( RTC_I2C_ADDR | I2C_MODE_WRITE );
    i2cWriteByte( RTC_SECONDS_ADDR );
    
    i2cWriteByte( int_to_bcd(sec) );
    i2cWriteByte( int_to_bcd(min) );
    i2cWriteByte( int_to_bcd(hour) );
    
    i2cStop();
}

void ds1307_get_date( uint8_t *day, uint8_t *month, uint8_t *year ) {
    i2cStart();
    
    i2cWriteByte( RTC_I2C_ADDR | I2C_MODE_WRITE );
    i2cWriteByte( RTC_DATE_ADDR );
    
    i2cReStart();
    
    i2cWriteByte( RTC_I2C_ADDR | I2C_MODE_READ );
    
    *day = bcd_to_int( i2cReadByte() );
    i2cReadAck();
    *month = bcd_to_int( i2cReadByte() );
    i2cReadAck();
    *year = bcd_to_int( i2cReadByte() );
    i2cReadNoAck();
    
    i2cStop();
}

void ds1307_get_time( uint8_t *hour, uint8_t *min, uint8_t *sec ) {
    i2cStart();
    
    i2cWriteByte( RTC_I2C_ADDR | I2C_MODE_WRITE );
    i2cWriteByte( RTC_SECONDS_ADDR );
    
    i2cReStart();
    
    i2cWriteByte( RTC_I2C_ADDR | I2C_MODE_READ );
    
    *sec = bcd_to_int( i2cReadByte() );
    i2cReadAck();
    *min = bcd_to_int( i2cReadByte() );
    i2cReadAck();
    *hour = bcd_to_int( i2cReadByte() );
    i2cReadNoAck();
    
    i2cStop();
}

/*==================[fin del archivo]========================================*/