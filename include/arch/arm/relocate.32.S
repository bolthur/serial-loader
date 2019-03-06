
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

.macro relocate address
  // relocate loader from soc address to link address
  ldr r3, =\address
  ldr r4, =__loader_start
  ldr r9, =__loader_end
  1:
    // load 4 values into register from r3
    ldmia r3!, {r5 - r8}
    // write multiple to loader start
    stmia r4!, {r5 - r8}
    // loop until loader end
    cmp r4, r9
    blo 1b
.endm
