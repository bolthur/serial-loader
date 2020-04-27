
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

.macro relocate address
  // relocate loader from soc address to link address
  ldr x3, =\address
  ldr x4, =__loader_start
  ldr x6, =__loader_end
  1:
    // load value into register x4 from x3
    ldr x5, [ x3 ], #8
    // store value from register x5 at address within x4
    str x5, [ x4 ], #8
    // loop until loader end
    cmp x4, x6
    blo 1b
.endm
