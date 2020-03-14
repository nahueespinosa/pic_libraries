/*============================================================================
 * Licencia:
 * Autor:       Nahuel Espinosa
 * Fecha:       20/10/2019
 *===========================================================================*/
/** @file
 * @brief	Contiene macros y definiciones para el módulo DS1307
 * 
 * Aqui se definen macros, tipos de datos y declaración de funciones
 */
#ifndef _DS1307_H_
#define _DS1307_H_

/*==================[inclusiones]============================================*/
#include <stdint.h>

/*==================[macros]=================================================*/

/*==================[declaraciones de funciones externas]====================*/
void ds1307_config  (void);
void ds1307_set_date(uint8_t day , uint8_t month, uint8_t year);
void ds1307_set_time(uint8_t hour, uint8_t min  , uint8_t sec );
void ds1307_get_date(uint8_t *day , uint8_t *month, uint8_t *year);
void ds1307_get_time(uint8_t *hour, uint8_t *min  , uint8_t *sec );

/*==================[fin del archivo]========================================*/
#endif /* _DS1307_H_ */