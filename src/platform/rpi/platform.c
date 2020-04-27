
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
 * @brief platform boot parameter
 */
uint32_t platform_boot_parameter[ 3 ];

/**
 * @brief Kernel entry function pointer
 */
typedef void ( *kernel_entry )( uint32_t r0, uint32_t r1, uint32_t r2 );

/**
 * @brief Boot loaded kernel
 */
void platform_boot_kernel( void ) {
  // get pointer to kernel start function
  kernel_entry kernel = ( kernel_entry )SOC_LOAD_ADDRESS;

  // call kernel startup function
  kernel(
    platform_boot_parameter[ 0 ],
    platform_boot_parameter[ 1 ],
    platform_boot_parameter[ 2 ]
  );
}


