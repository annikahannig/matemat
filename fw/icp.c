
#include "config.h"

#include <avr/interrupt.h>
#include <avr/io.h>

volatile uint8_t   _icp_edge = 0;
volatile uint16_t  _icp_overflow_count = 0;
volatile uint16_t  _icp_t0 = 0;
volatile uint16_t  _icp_t1 = 0;
volatile uint8_t   _icp_has_data = 0;


/**
 * Trigger on rising and falling edge
 */
ISR(TIMER1_CAPT_vect)
{
  if(_icp_edge == 0) {
    _icp_t0 = ICR1;
    _icp_edge = 1;
    TCCR1B &= ~(1<<ICES1); // Trigger on falling edge
  }
  else {
    // Stop counting
    TCCR1B = 0x00;

    // Save capture
    _icp_t1 = ICR1;
    _icp_edge = 0;
    _icp_has_data = 1;
  }
}

/**
 * Count overflows
 */
ISR(TIMER1_OVF_vect)
{
  _icp_overflow_count++;
}


/**
 * Initialize
 */
void ICP_init()
{
  TCCR1A = 0; // Normal mode / no PWM
  TCCR1B |= (1<<ICES1) | (1<<CS10); // Trigger on rising edge, no prescale
  TIMSK1 |= (1<<ICIE1) | (1<<TOIE1); // Enable capture and overflow interrupts
}


/**
 * Start measure
 */
void ICP_start()
{
  _icp_edge = 0;
  _icp_overflow_count = 0;
  _icp_has_data = 0;
  TCCR1B |= (1<<ICES1) | (1<<CS10); // Rising edge, no prescale
  TCNT1 = 0; // Reset counter
}


inline uint32_t _cycles_to_us(uint32_t t)
{
  return t / (F_CPU / 10e6);
}


/**
 * Wait for result
 */
uint32_t ICP_read_us()
{
  while(!_icp_has_data) {} // Wait until data

  if(_icp_overflow_count > 10) { // out of range.
    return -1;
  }

  uint32_t t_len = _icp_overflow_count * 65535;
  uint32_t t_diff = (t_len + _icp_t1) - _icp_t0;
  uint32_t t_diff_us = _cycles_to_us(t_diff);

  return t_diff_us;
}


