
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
#include <stdarg.h>
#include <stddef.h>

#include "loader/memcpy.h"

#include "platform/rpi/mailbox/property.h"
#include "platform/rpi/mailbox/mailbox.h"

/**
 * @brief property tag buffer, which needs to be aligned to 16 byte boundary
 */
static int32_t property_buffer[ 256 ] ALIGNED( 16 );

/**
 * @brief property tag buffer index
 */
static int32_t property_buffer_idx = 0;

/**
 * @brief Initialize mailbox property process
 */
void mailbox_property_init( void ) {
  // Add startup size
  property_buffer[ PT_OSIZE ] = 12;

  // process request, everything else seems to be reserved
  property_buffer[ PT_OREQUEST_OR_RESPONSE ] = 0;

  // first available data slot
  property_buffer_idx = 2;

  // 0-tag to terminate list after init
  property_buffer[ property_buffer_idx ] = 0;
}

/**
 * @brief Add tag to mailbox property process
 *
 * @param tag Tag to add
 * @param ... Further data depending on tag to be added
 */
void mailbox_property_add_tag( rpi_mailbox_tag_t tag, ... ) {
  va_list vl;
  va_start( vl, tag );

  // start with adding the tag itself
  property_buffer[ property_buffer_idx++ ] = tag;

  switch( tag ) {
    case TAG_GET_FIRMWARE_VERSION:
    case TAG_GET_BOARD_MODEL:
    case TAG_GET_BOARD_REVISION:
    case TAG_GET_BOARD_MAC_ADDRESS:
    case TAG_GET_BOARD_SERIAL:
    case TAG_GET_ARM_MEMORY:
    case TAG_GET_VC_MEMORY:
    case TAG_GET_DMA_CHANNELS:
      // provide 8-byte buffer for response
      property_buffer[ property_buffer_idx++ ] = 8;
      property_buffer[ property_buffer_idx++ ] = 0; // request
      property_buffer_idx += 2;
      break;

    case TAG_GET_CLOCK_STATE:
    case TAG_GET_CLOCK_RATE:
    case TAG_GET_MAX_CLOCK_RATE:
    case TAG_GET_MIN_CLOCK_RATE:
    case TAG_GET_TURBO:
      // provide 8-byte buffer for response
      property_buffer[ property_buffer_idx++ ] = 8;
      property_buffer[ property_buffer_idx++ ] = 0; // request
      property_buffer[ property_buffer_idx++ ] = va_arg( vl, int32_t ); // clock
      property_buffer_idx += 2;
      break;

    case TAG_GET_CLOCKS:
    case TAG_GET_COMMAND_LINE:
      // provide a 256-byte buffer
      property_buffer[ property_buffer_idx++ ] = 256;
      property_buffer[ property_buffer_idx++ ] = 0; // request
      property_buffer_idx += 256 >> 2;
      break;

    case TAG_ALLOCATE_BUFFER:
      property_buffer[ property_buffer_idx++ ] = 8;
      property_buffer[ property_buffer_idx++ ] = 0; // request
      property_buffer[ property_buffer_idx++ ] = va_arg( vl, int32_t );
      property_buffer_idx += 1;
      break;

    case TAG_GET_PHYSICAL_SIZE:
    case TAG_SET_PHYSICAL_SIZE:
    case TAG_TEST_PHYSICAL_SIZE:
    case TAG_GET_VIRTUAL_SIZE:
    case TAG_SET_VIRTUAL_SIZE:
    case TAG_TEST_VIRTUAL_SIZE:
    case TAG_GET_VIRTUAL_OFFSET:
    case TAG_SET_VIRTUAL_OFFSET:
      property_buffer[ property_buffer_idx++ ] = 8;
      property_buffer[ property_buffer_idx++ ] = 0; // request

      if(
        tag == TAG_SET_PHYSICAL_SIZE
        || tag == TAG_SET_VIRTUAL_SIZE
        || tag == TAG_SET_VIRTUAL_OFFSET
        || tag == TAG_TEST_PHYSICAL_SIZE
        || tag == TAG_TEST_VIRTUAL_SIZE
      ) {
        property_buffer[ property_buffer_idx++ ] = va_arg( vl, int32_t ); // width
        property_buffer[ property_buffer_idx++ ] = va_arg( vl, int32_t ); // height
      } else {
        property_buffer_idx += 2;
      }
      break;


    case TAG_GET_ALPHA_MODE:
    case TAG_SET_ALPHA_MODE:
    case TAG_GET_DEPTH:
    case TAG_SET_DEPTH:
    case TAG_GET_PIXEL_ORDER:
    case TAG_SET_PIXEL_ORDER:
    case TAG_GET_PITCH:
      property_buffer[ property_buffer_idx++ ] = 4;
      property_buffer[ property_buffer_idx++ ] = 0; // request

      if(
        tag == TAG_SET_DEPTH
        || tag == TAG_SET_PIXEL_ORDER
        || tag == TAG_SET_ALPHA_MODE
      ) {
        // color depth, bits-per-pixel \ pixel order state
        property_buffer[ property_buffer_idx++ ] = va_arg( vl, int32_t );
      } else {
        property_buffer_idx += 1;
      }
      break;

    case TAG_GET_OVERSCAN:
    case TAG_SET_OVERSCAN:
      property_buffer[ property_buffer_idx++ ] = 16;
      property_buffer[ property_buffer_idx++ ] = 0; // request

      if( tag == TAG_SET_OVERSCAN ) {
        property_buffer[ property_buffer_idx++ ] = va_arg( vl, int32_t ); // top pixels
        property_buffer[ property_buffer_idx++ ] = va_arg( vl, int32_t ); // bottom pixels
        property_buffer[ property_buffer_idx++ ] = va_arg( vl, int32_t ); // left pixels
        property_buffer[ property_buffer_idx++ ] = va_arg( vl, int32_t ); // right pixels
      } else {
        property_buffer_idx += 4;
      }
      break;

    case TAG_SET_CLOCK_RATE:
      property_buffer[ property_buffer_idx++ ] = 12; // request size
      property_buffer[ property_buffer_idx++ ] = 8; // response size

      property_buffer[ property_buffer_idx++ ] = va_arg( vl, int32_t ); // clock
      property_buffer[ property_buffer_idx++ ] = va_arg( vl, int32_t ); // Hz
      property_buffer[ property_buffer_idx++ ] = va_arg( vl, int32_t ); // turbo

      property_buffer_idx += 2;
      break;

    case TAG_SET_CLOCK_STATE:
      property_buffer[ property_buffer_idx++ ] = 8; // request size
      property_buffer[ property_buffer_idx++ ] = 8; // response size

      // push request parameters
      property_buffer[ property_buffer_idx++ ] = va_arg( vl, int32_t ); // clock
      property_buffer[ property_buffer_idx++ ] = va_arg( vl, int32_t ); // state

      property_buffer_idx += 2;
      break;

    default:
      // Unsupported tag, just remove it
      property_buffer_idx--;
      break;
  }

  // Null termination of tag buffer
  property_buffer[ property_buffer_idx ] = 0;

  va_end( vl );
}

/**
 * @brief Execute mailbox property process
 *
 * @return uint32_t mailbox read result after write
 */
uint32_t mailbox_property_process( void ) {
  uint32_t result;

  // set correct size
  property_buffer[ PT_OSIZE ] = ( property_buffer_idx + 1 ) << 2;
  property_buffer[ PT_OREQUEST_OR_RESPONSE ] = 0;

  // write to mailbox
  mailbox_write( MAILBOX0_TAGS_ARM_TO_VC, (uint32_t)property_buffer );

  // read result
  result = mailbox_read( MAILBOX0_TAGS_ARM_TO_VC );

  // return result
  return result;
}

/**
 * @brief Read tag from previous executed mailbox property process
 *
 * @param tag tag to read from mailbox property process
 * @return rpi_mailbox_property_t* Pointer to structure of tag or NULL
 */
rpi_mailbox_property_t* mailbox_property_get( rpi_mailbox_tag_t tag ) {
  // property structure for return and tag buffer
  static rpi_mailbox_property_t property;
  int32_t* tag_buffer = NULL;

  // setup property structure to return
  property.tag = tag;

  // Get the tag from the buffer and start with first available tag position
  int32_t idx = 2;

  while( idx < ( property_buffer[ PT_OSIZE ] >> 2 ) ) {
    // test tag
    if( property_buffer[ idx ] == ( int32_t )tag ) {
      tag_buffer = &property_buffer[ idx ];
      break;
    }

    // progress with next tag if we haven't yet discovered the wanted tag
    idx += ( property_buffer[ idx + 1 ] >> 2 ) + 3;
  }

  // nothing found, return NULL
  if( NULL == tag_buffer ) {
    return NULL;
  }

  // copy necessary data into return structure
  property.byte_length = tag_buffer[ T_ORESPONSE ] & 0xFFFF;
  memcpy(
    property.data.buffer_u8,
    &tag_buffer[ T_OVALUE ],
    ( size_t )property.byte_length
  );

  // return address of static return
  return &property;
}
