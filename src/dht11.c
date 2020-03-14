/*============================================================================
 * Licencia:
 * Autor:       Nahuel Espinosa
 * Fecha:       20/10/2019
 *===========================================================================*/
/** @file
 * @brief	Contiene las definiciones para el módulo DHT11
 * 
 * Aqui se encuentra la implementación de funciones, los defines, macros, 
 * datos internos y externos, declaraciones y definiciones de funciones
 * internas y definiciones de funciones externas
 */

/*==================[inclusiones]============================================*/
#include <xc.h>
#include "dht11.h"

/*==================[definiciones y macros]==================================*/
#define DHT11_TIMEOUT     (100)     // Timeout en microsegundos
#define DHT11_DATA_SIZE   (5)

#define TRUE  1
#define FALSE 0

#define _XTAL_FREQ        (4000000L)

#define dht11_GPIO_Low()      PIN_DHT11  = 0; TRIS_DHT11 = 0;
#define dht11_GPIO_High()     TRIS_DHT11 = 1;
#define dht11_GPIO_Read()     PIN_DHT11

// Utiliza un timer para identificar los tiempos de cada bit de datos y el timeout
#define dht11_TMR_Reset()     TMR0 = 0
#define dht11_TMR_Read()      TMR0
#define dht11_TMR_Config()    OPTION_REGbits.T0CS = 0; OPTION_REGbits.PSA = 1;

/*==================[definiciones de datos internos]=========================*/
static uint8_t dht11_byte[DHT11_DATA_SIZE];
static uint8_t dht11_aux;

/*==================[definiciones de funciones internas]=====================*/
/**
 * @brief       Lee un byte enviado por el módulo DHT11
 * @return      1 si la recepción fue correcta
 *              0 si hubo timeout
 * @note        Guarda el resultado en una variable global auxiliar (dht11_aux)
 */
static bit dht11_read_byte() {
    uint8_t i;
    dht11_aux = 0;
    
    // Recibo 8 bits
    for( i = 0 ; i < 8 ; i++ ) {
        // Espero flanco ascendente, el pulso en bajo siempre es de 50 us
        dht11_TMR_Reset();
        while( !dht11_GPIO_Read() ) {
            if( dht11_TMR_Read() > DHT11_TIMEOUT ) return FALSE;
        }
        
        // Espero flanco descendente y mido la duración del pulso en alto
        // Si el pulso en alto dura 28 us es un 0, si dura 70 us es un 1
        dht11_TMR_Reset();
        while( dht11_GPIO_Read() ) {
            if( dht11_TMR_Read() > DHT11_TIMEOUT ) return FALSE;
        }
        
        // Si el pulso en alto supera los 50 us es un 1 lógico, sino es un 0
        // Se reciben primero los bits más significativos
        // if( dht11_TMR_Read() > 50 ) dht11_aux |= 0x01;
        // byte_aux <<= 1;
        // Se escribe en assembler para no perder datos por la demora de ejecución        
        asm("  movlw  0x32          ");
        asm("  subwf  TMR0,w        ");
        asm("  rlf    _dht11_aux,f  ");        
    }
    return TRUE;
}

/*==================[definiciones de funciones externas]=====================*/
/**
 * @brief       Configura e inicializa el pin de comunicación y el timer
 * @return      Nada
 */
void dht11_config (void) {
	dht11_GPIO_High();
    dht11_TMR_Config()
}

/**
 * @brief       Lee los datos del módulo DHT11
 * @param[in]   *phum:  Dirección de la variable donde guardar la humedad
 * @param[in]   *ptemp: Dirección de la variable donde guardar la temperatura
 * @return      1 si la recepción fue correcta
 *              0 si hubo timeout o error de checksum
 */
bit dht11_read (float *phum, float *ptemp) {
    uint8_t i;
    
    // Start signal por lo menos de 18 ms según datasheet
    dht11_GPIO_Low();
	__delay_ms(18);
	dht11_GPIO_High();
    
    // Espero flanco descendente de respuesta
    dht11_TMR_Reset();
    while(  dht11_GPIO_Read() ) {
        if( dht11_TMR_Read() > DHT11_TIMEOUT ) return FALSE; // Devuelvo falso si hubo timeout
    }
    
    // Espero flanco ascendente de respuesta (80 us)
    dht11_TMR_Reset();
    while( !dht11_GPIO_Read() ) {
        if( dht11_TMR_Read() > DHT11_TIMEOUT ) return FALSE; // Devuelvo falso si hubo timeout
    }
    
    // Espero flanco descendente de respuesta (80 us)
    dht11_TMR_Reset();
    while(  dht11_GPIO_Read() ) {
        if( dht11_TMR_Read() > DHT11_TIMEOUT ) return FALSE; // Devuelvo falso si hubo timeout
    }
    
    // El módulo empieza a transmitir los datos (40 bits):
    //     8 bits de parte entera de RH, 8 bits de parte decimal de RH
    //     8 bits de parte entera de T , 8 bits de parte decimal de T
    //     8 bits de checksum
    for( i = 0 ; i < DHT11_DATA_SIZE ; i++ ) {
        if( !dht11_read_byte() ) return FALSE;  // Devuelvo falso si hubo timeout
        dht11_byte[i] = dht11_aux;
    }
    
    // Checkeo de integridad de los datos
    uint8_t checksum;
	checksum = dht11_byte[0] + dht11_byte[1] + dht11_byte[2] + dht11_byte[3];
	if( checksum != dht11_byte[4] ) {
		return FALSE;   // Devuelvo falso si hubo un error de checksum
	}
    
    // Formateo los datos
    *phum 	= ((float)dht11_byte[0]) + ((float)dht11_byte[1])/10;
    *ptemp 	= ((float)dht11_byte[2]) + ((float)dht11_byte[3])/10;
	return TRUE;
}

/*==================[fin del archivo]========================================*/