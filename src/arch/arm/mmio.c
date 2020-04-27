
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

#include <stdint.h>

/**
 * @brief Write to memory mapped I/O
 *
 * @param reg register/address to write to
 * @param data data to write
 */
void OPT_NONE mmio_write( uint32_t reg, uint32_t data ) {
  *( volatile uint32_t* )( ( uint32_t )reg ) = data;
}

/**
 * @brief Read from memory mapped I/O
 *
 * @param reg register/address to read
 * @return uint32_t data from memory mapped I/O
 */
uint32_t OPT_NONE mmio_read( uint32_t reg ) {
  return *( volatile uint32_t* )( ( uint32_t )reg );
}
