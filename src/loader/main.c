
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
#include "loader/platform.h"
#include "loader/entry.h"

extern void boot_start( void );

/**
 * @brief Loader main function
 */
void loader_main() {
  // local variables
  uint32_t size, loader_end_address, type;
  bool processing = false;

  // setup serial connection
  serial_init();

  // loop until kernel dropped in
  while ( true ) {
    // handle only if not processing
    if ( ! processing ) {
      // some initial output
      printf(
        "%s\r\nfunction located at: 0x%08x\r\nkernel loaded to: 0x%08x\r\ninitrd loaded to: 0x%08x\r\nboot_start: 0x%08x\r\n",
        PACKAGE_STRING, loader_main, SOC_LOAD_ADDRESS, INITRD_LOAD_ADDRESS, boot_start
      );

      // flush serial device
      serial_flush();

      // send end of text to signal wait for kernel
      serial_puts( "\x03\x03\x03" );

      // set processing flag
      processing = true;
    }

    // get type
    uint16_t tmp_type = ( uint32_t )serial_getc();
    tmp_type |= ( uint16_t )( serial_getc() << 8 );

    // check for go command
    uint8_t *check_go = ( uint8_t* )&tmp_type;
    if(
      'G' == check_go[ 0 ]
      && 'O' == check_go[ 1 ]
    ) {
      // boot loaded kernel
      printf( "Trying to boot received kernel\r\n" );
      platform_boot_kernel();

      // break out of loop
      break;
    }

    type = tmp_type;
    type |= ( uint32_t )( serial_getc() << 16 );
    type |= ( uint32_t )( serial_getc() << 24 );

    // check type
    if (
      FILE_TYPE_INITRD != type
      && FILE_TYPE_KERNEL != type
    ) {
      serial_puts( "ER" );
      processing = false;
      continue;
    }

    // send okay
    serial_puts( "OK" );

    // get kernel size in bytes
    size = ( uint32_t )serial_getc();
    size |= ( uint32_t )( serial_getc() << 8 );
    size |= ( uint32_t )( serial_getc() << 16 );
    size |= ( uint32_t )( serial_getc() << 24 );

    // calculate end address
    loader_end_address = SOC_LOAD_ADDRESS + size;
    if ( type == FILE_TYPE_INITRD ) {
      loader_end_address = INITRD_LOAD_ADDRESS + size;
    }

    // check for possible overflow and skip rest
    if ( loader_end_address > ( uint32_t )&__loader_start ) {
      serial_puts( "ER" );
      processing = false;
      continue;
    }

    // signal OK
    serial_puts( "OK" );

    // pointer to kernel at system load address
    uint8_t *file = ( uint8_t* )SOC_LOAD_ADDRESS;
    if ( type == FILE_TYPE_INITRD ) {
      file = ( uint8_t* )INITRD_LOAD_ADDRESS;
    }
    uint32_t count = 0;

    // loop until size is 0
    while( size-- ) {
      // every 4 byte sent okay
      if ( ! ( count++ % 250 ) ) {
        serial_puts( "OK" );
      }

      // store current byte
      *file++ = serial_getc();
    }
  }

  // endless loop should not be reached
  while( true ) {}
}
