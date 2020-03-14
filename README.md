# PIC Libraries
Libraries for interfacing PIC16F887 with several peripherals.
Most of them were programmed for specific projects of high school
students and I thought of sharing them as they might be useful as
a basic start in hobbyist projects.

## Index
- DHT11
    - Basic ultra low-cost temperature and humidity sensor. It uses a
    one-wire protocol and this driver's implementation contemplates 
    the use of down to 4MHz crystal as clock source.
- DS1307
    - Serial real-time clock, full binary-coded decimal (BCD) clock/calendar 
    plus 56 bytes of NV SRAM.
- MAX7219
    - Serial input/output common-cathode display drivers. Implements functions
    for drawing in an 8x8 led matrix.
- MSSP
    - Driver for configuration and usage of the "Master Synchronous Serial Port"
    module for PIC16F887.
- LCD through I2C
    - Functions to configure and control an LCD connected to an IO expander (PCF8574).

## Usage
The compiler used was XC8 v1.45 in MPLABX IDE.

Copy the necessary *.c and *.h files in your projects directory and include the 
header files.

```C
#include "dht11.h"

void main(void) {
    float hum, temp;
    dht11_config();

    while(1) {
        dht11_read(&hum, &temp);
        __delay_ms(100);
    }
}
```

Be sure to check the header file of each device to correctly select the GPIOs
according to your board.

```C
#define PIN_DHT11   PORTBbits.RB4
#define TRIS_DHT11  TRISBbits.TRISB4
```