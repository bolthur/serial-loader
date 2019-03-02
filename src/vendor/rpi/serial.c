
/**
 * Copyright (C) 2019 bolthur project.
 *
 * This file is part of bolthur/serial-loader.
 *
 * bolthur/serial-loader is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * bolthur/serial-loader is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with bolthur/serial-loader.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>

#include "loader/printf.h"

#include "arch/arm/delay.h"
#include "arch/arm/mmio.h"

#include "vendor/rpi/mailbox/property.h"
#include "vendor/rpi/peripheral.h"
#include "vendor/rpi/gpio.h"

void serial_init( void ) {
  // Disable UART0.
  mmio_write( PERIPHERAL_BASE + UARTCR, 0 );

  // Setup the GPIO pin 14 && 15
  uint32_t ra = mmio_read( PERIPHERAL_BASE + GPFSEL1 );
  ra = ( uint32_t )( ( int32_t ) ra & ~( 7 << 12 ) );
  ra |= 4 << 12;
  ra = ( uint32_t )( ( int32_t ) ra & ~( 7 << 15 ) );
  ra |= 4 << 15;
  mmio_write( PERIPHERAL_BASE + GPFSEL1, ra );

  // Disable pull up/down for all GPIO pins & delay for 150 cycles.
  mmio_write( PERIPHERAL_BASE + GPPUD, 0 );
  delay( 150 );

  // Disable pull up/down for pin 14,15 & delay for 150 cycles.
  mmio_write( PERIPHERAL_BASE + GPPUDCLK0, ( 1 << 14 ) | ( 1 << 15 ) );
  delay( 150 );

  // Write 0 to GPPUDCLK0 to make it take effect.
  mmio_write( PERIPHERAL_BASE + GPPUDCLK0, 0 );

  // Clear pending interrupts.
  mmio_write( PERIPHERAL_BASE + UARTICR, 0x7FF );

  // Set integer & fractional part of baud rate.
  // Divider = UART_CLOCK/(16 * Baud)
  // Fraction part register = (Fractional part * 64) + 0.5
  // UART_CLOCK = 3000000; Baud = 115200.

  // query mailbox to get uart clock rate
  mailbox_property_init();
  mailbox_property_add_tag( TAG_GET_CLOCK_RATE, TAG_CLOCK_UART );
  mailbox_property_process();

  // get clock rate property
  rpi_mailbox_property_t *p = mailbox_property_get( TAG_GET_CLOCK_RATE );
  uint32_t clock_rate = p->data.buffer_u32[ 1 ];

  // calculate divider
  float divider = ( float )clock_rate / ( 16 * 115200 );
  // cast to integer for later write to ibrd
  uint32_t brd = ( uint32_t )divider;

  // calculate fractional
  float fractional = divider - ( float )brd;
  // calculate fractional for later write to fbrd
  uint32_t frd = ( uint32_t )( ( fractional * 64 ) + 0.5 );

  // write baud rate
  mmio_write( PERIPHERAL_BASE + UARTIBRD, brd );
  mmio_write( PERIPHERAL_BASE + UARTFBRD, frd );

  // Enable FIFO & 8 bit data transmissio (1 stop bit, no parity).
  mmio_write( PERIPHERAL_BASE + UARTLCRH, ( 1 << 4 ) | ( 1 << 5 ) | ( 1 << 6 ) );

  // Mask all interrupts.
  mmio_write( PERIPHERAL_BASE + UARTIMSC, ( 1 << 1 ) | ( 1 << 4 ) | ( 1 << 5 ) | ( 1 << 6 ) | ( 1 << 7 ) | ( 1 << 8 ) | ( 1 << 9 ) | ( 1 << 10 ) );

  // Enable UART0, receive & transfer part of UART.
  mmio_write( PERIPHERAL_BASE + UARTCR, ( 1 << 0 ) | ( 1 << 8 ) | ( 1 << 9 ) );
}

/**
 * @brief Put character to serial
 *
 * @param c character to put
 */
void serial_putc( uint8_t c ) {
  // Wait for UART to become ready to transmit.
  while ( 0 != ( mmio_read( PERIPHERAL_BASE + UARTFR ) & ( 1 << 5 ) ) ) { }
  mmio_write( PERIPHERAL_BASE + UARTDR, ( uint32_t ) c );
}

/**
 * @brief Get character from serial
 *
 * @return uint8_t Character from serial
 */
uint8_t serial_getc( void ) {
  // Wait for UART to become ready for read
  while ( mmio_read( PERIPHERAL_BASE + UARTFR ) & ( 1 << 4 ) ) { }

  // return data
  return ( uint8_t )mmio_read( PERIPHERAL_BASE + UARTDR );
}

/**
 * @brief Flush serial
 */
void serial_flush( void ) {
  // read from uart until as long as something is existing to flush
  while ( ! ( mmio_read( PERIPHERAL_BASE + UARTFR ) & ( 1 << 4 ) ) ) {
    serial_getc();
  }
}

/**
 * @brief Method to print string to serial
 *
 * @param str string to print
 */
void serial_puts( const char* str ) {
  while( *str ) {
    serial_putc( *str++ );
  }
}
