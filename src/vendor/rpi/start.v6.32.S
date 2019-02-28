
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

.section .text.boot

.global boot_start
boot_start:
  // setup temporary stack
  ldr r3, =boot_start
  mov sp, r3

  // FIXME: Clear bss section
  // FIXME: Save boot parameter somewhere
  // FIXME: Relocate loader code from fixed load address to different address

  ldr r3, =loader_main
  blx r3

halt:
  wfe // equivalent of x86 HLT instruction
  b halt

