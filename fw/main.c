
/**
 * Matemat Firmware
 * ----------------
 *
 * Measure distance and write current distance for
 * each slot in serial console.
 *
 */

#include "config.h"

#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>
#include <stdio.h>

#include "uart.h"
#include "icp.h"

#define ECHO_IN   PINB
#define ECHO_PORT PORTB
#define ECHO_DDR  DDRB

#define ECHO_0    (1<<PB0)

#define TRIGGER_PORT PORTB
#define TRIGGER_DDR  DDRB

#define TRIGGER_0 (1<<PB1)
#define TRIGGER_1 (1<<PB2)
#define TRIGGER_2 (1<<PB3)
#define TRIGGER_3 (1<<PB4)
#define TRIGGER_4 (1<<PB5)



/**
 * Trigger measurement
 */
void HCSR04_trigger(uint8_t port)
{
  TRIGGER_PORT |= port;
  _delay_us(10);
  TRIGGER_PORT &= ~port;
}



int main(void) {
  uint32_t pulse_len = 0;

  // Setup IO
  ECHO_DDR &= ~ECHO_0;
  TRIGGER_DDR |= TRIGGER_0;

  // Setup UART
  USART_init();

  // Setup input capture
  ICP_init();

  sei();

  // Measurement loop
  char buffer[80];
  for(;;) {
    ICP_start();
    HCSR04_trigger(TRIGGER_0);
    pulse_len = ICP_read_us();

    sprintf(buffer, " --> %.2f cm", (double)pulse_len/58.0/10.0);
    USART_writeln(buffer);

    _delay_ms(100);
  }
}



