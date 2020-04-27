
/**
 * Copyright (C) 2019 - 2020 bolthur project.
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

#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>

#include "loader/serial.h"
#include "loader/strrev.h"
#include "loader/strlen.h"
#include "loader/itoa.h"
#include "loader/utoa.h"

static void print( const char* data, size_t length, int32_t pad0, int32_t pad ) {
  const unsigned char* bytes = ( const unsigned char * )data;

  // print padding if set
  for ( size_t i = 0; pad > 0 && i < ( ( size_t )pad - length ); i++ ) {
    serial_putc( pad0 ? '0' : ' ' );
  }

  // print data
  for ( size_t i = 0; i < length; i++ ) {
    serial_putc( bytes[ i ] );
  }
}

void printf( const char* restrict format, ... ) {
  va_list parameters;
  va_start( parameters, format );

  size_t amount, len;
  char c, buf[ 30 ];
  int32_t n;
  uint32_t un;
  char *p;
  int32_t pad, pad0, radix;

  while ( *format != '\0' ) {
    pad = pad0 = 0;

    if ( format[ 0 ] != '%' || format[ 1 ] == '%' ) {
      if ( format[ 0 ] == '%' ) {
        format++;
      }

      amount = 1;

      while ( format[ amount ] && format[ amount ] != '%' ) {
        amount++;
      }

      print( format, amount, pad0, pad );
      format += amount;
      continue;
    }

    const char* format_begun_at = format++;

    if ( *format == '0' ) {
      pad0 = 1;
      format++;
    }
    if ( *format >= '0' && *format <= '9' ) {
      pad = *format - '0';
      format++;
    }

    switch ( *format ) {
      case 'c':
        format++;
        // char promotes to int
        c = ( char )va_arg( parameters, int );
        print( &c, sizeof( c ), pad0, pad );
        break;

      case 's':
        format++;
        const char* str = va_arg( parameters, const char* );
        len = strlen( str );
        print( str, len, pad0, pad );
        break;

      case 'u':
      case 'p':
        if ( 'p' == *format ) {
          un = ( uint32_t )va_arg( parameters, void* );
          radix = 16;
        } else {
          un = va_arg( parameters, uint32_t );
          radix = 10;
        }

        if ( ! un ) {
          un = 0;
        }

        p = utoa( un, buf, radix, false );
        len = strlen( p );

        print( p, len, pad0, pad );
        format++;
        break;

      case 'x':
      case 'X':
      case 'd':
      case 'i':
      case 'o':
        if ( 'd' == *format || 'i' == *format ) {
          radix = 10;
        } else if ( 'o' == *format ) {
          radix = 8;
        } else {
          radix = 16;
        }

        n = va_arg( parameters, int32_t );
        if ( ! n ) {
          n = 0;
        }

        p = itoa( n, buf, radix, 'X' == *format );
        len = strlen( p );

        print( p, len, pad0, pad );
        format++;
        break;

      default:
        format = format_begun_at;
        len = strlen(format);
        print( format, len, pad0, pad );
        format += len;
    }
  }

  va_end( parameters );
}
