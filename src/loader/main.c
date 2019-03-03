
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

#include <stdbool.h>
#include <stdint.h>

#include "loader/serial.h"
#include "loader/printf.h"
#include "loader/vendor.h"
#include "loader/entry.h"

/**
 * @brief Loader main function
 */
void loader_main() {
  // local variables
  uint32_t size, loader_end_address;

  // setup serial connection
  serial_init();

  // loop until kernel dropped in
  while ( true ) {
    // some initial output
    printf( "%s\r\n", PACKAGE_STRING );

    // flush serial device
    serial_flush();

    // send end of text to signal wait for kernel
    serial_puts( "\x03\x03\x03" );

    // check for start of kernel via start of text signal
    if ( '\x02' != serial_getc() ) {
      continue;
    }

    // get kernel size in bytes
    size = ( uint32_t )serial_getc();
    size |= ( uint32_t )( serial_getc() << 8 );
    size |= ( uint32_t )( serial_getc() << 16 );
    size |= ( uint32_t )( serial_getc() << 24 );

    // calculate end address
    loader_end_address = SOC_LOAD_ADDRESS + size;

    // check for possible overflow
    if ( loader_end_address > ( uint32_t )&__loader_start ) {
      printf( "Kernel to load is to big for this tool!\r\n" );
      break;
    }

    // pointer to kernel at system load address
    uint8_t *kernel = ( uint8_t* )SOC_LOAD_ADDRESS;

    // loop until size is 0
    while( size-- > 0 ) {
      *kernel++ = serial_getc();
    }

    // boot loaded kernel
    printf( "Booting received kernel with size of %d bytes\r\n", size );
    vendor_boot_kernel();

    // break out of loop
    break;
  }

  // endless loop should not be reached
  while( true ) {}
}
