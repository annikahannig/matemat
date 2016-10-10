
#include "uart.h"

#include <avr/io.h>

/**
 * Transmit a single char
 */
void USART_tx(const unsigned char data)
{
    /* Wait for empty transmit buffer */
    while(!(UCSR0A & (1<<UDRE0))) {
        // do_nothing() tm
    };

    /* Put data into buffer, sends the data */
    UDR0 = data;
}

/**
 * Send a string
 */
void USART_puts(const char* s)
{
    while (*s)
    {
        USART_tx(*s);
        s++;
    }
}

/**
 * Write a single line
 */
void USART_writeln(const char *s)
{
    USART_puts(s);
    USART_tx('\n');
    USART_tx('\r');
}

/**
 * Initialize UART
 */
void USART_init()
{
    /* Set baud rate */
    UBRR0H = (unsigned char)(UBRR_VAL>>8);
    UBRR0L = (unsigned char)(UBRR_VAL & 0xff);

    UCSR0A &= ~(1<<U2X0); // 1x data rate
    UCSR0C = (1<<UCSZ01)|(3<<UCSZ00);
    /* Enable transmitter only */
    UCSR0B |= (1<<TXEN0);
}



