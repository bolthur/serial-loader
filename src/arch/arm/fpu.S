
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

/**
 * @brief Method to enable floating point unit when fpu enabled flag is set
 */
.global fpu_enable
fpu_enable:
  #if defined( FPU_AVAILABLE )
    // read co processor access controll register into r0
    mrc p15, 0, r0, c1, c0, 2
    // enable full access for everyone
    orr r0, r0, #( 0xf << 20 )
    // write back changes
    mcr p15, 0, r0, c1, c0, 2

    // Set FPEXC.EN bit to enable fpu
    mov r0, #( 0x1 << 30 )
    // Transfer change to vfp system register
    vmsr fpexc, r0
  #endif
  bx lr
