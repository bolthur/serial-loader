
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

OUTPUT_FORMAT( elf32-littlearm )
OUTPUT_ARCH( arm )

ENTRY( boot_start )

GROUP(
  ../../arch/arm/v6/libv6.a
  ../../arch/arm/libarm.a
  ../../loader/libloader.a
  ../../vendor/rpi/librpi.a
)
INPUT(
  libgcc.a
)

SECTIONS {
  . = 0x8000;
  __loader_start = .;

  .text ALIGN( 4K ) : AT( ADDR( .text ) ) {
    *( .text.boot )
    *( .text )
  }

  .rodata ALIGN( 4K ) : AT( ADDR( .rodata ) ) {
    *(.rodata)
  }

  .data ALIGN( 4K ) : AT( ADDR( .data ) ) {
    *(.data)
  }

  .bss ALIGN( 4K ) : AT( ADDR( .bss ) ) {
    __bss_start = .;
    *(COMMON)
    *(.bss)
    __bss_end = .;
  }

  __loader_end = .;

  /DISCARD/ : {
    *(.comment)
    *(.eh_frame)
    *(.ARM.exidx*)
    *(.ARM.attributes*)
  }
}
