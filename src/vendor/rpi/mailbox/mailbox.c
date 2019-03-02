
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

#include "vendor/rpi/peripheral.h"
#include "vendor/rpi/mailbox/mailbox.h"

/**
 * @brief Read value from mailbox
 *
 * @param channel channel to read
 * @return uint32_t value from channel
 */
uint32_t mailbox_read( mailbox0_channel_t channel ) {
  // data and count
  uint32_t value = 0, count = 0;

  // build address to read from and store it within pointer
  volatile mailbox_t* mbox = ( volatile mailbox_t* )(
    PERIPHERAL_BASE + MAILBOX_OFFSET
  );

  // loop until right channel drops in
  while ( ( value & 0xF ) != channel ) {
    // wait while mailbox is empty
    while( mbox->status & MAILBOX_EMPTY ) {
      // break when it takes to much time
      if ( count++ > ( 1 << 25 ) ) {
        return 0xFFFFFFFF;
      }
    }

    // read value
    value = mbox->read;
  }

  // return value without channel information
  return value >> 4;
}

/**
 * @brief Write data to mailbox
 *
 * @param channel channel to use
 * @param data data to write
 */
void mailbox_write( mailbox0_channel_t channel, uint32_t data ) {
  // add channel to data to write
  data = ( uint32_t )( ( int32_t )data & ~0xF );
  data |= channel;

  // get mailbox address
  volatile mailbox_t *mbox = ( volatile mailbox_t* )(
    PERIPHERAL_BASE + MAILBOX_OFFSET
  );

  // wait for mailbox to be ready
  while( ( mbox->status & MAILBOX_FULL ) != 0 ) {
  }

  // write data to mailbox
  mbox->write = data;
}
