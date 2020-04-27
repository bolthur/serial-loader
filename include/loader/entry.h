
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

#if !defined( ENTRY_H )
#define ENTRY_H

#include <stdint.h>

#define FILE_TYPE_KERNEL 1
#define FILE_TYPE_INITRD 2

extern uint32_t __loader_start;
extern uint32_t __loader_end;

extern uint32_t __text_start;
extern uint32_t __text_end;

extern uint32_t __rodata_start;
extern uint32_t __rodata_end;

extern uint32_t __data_start;
extern uint32_t __data_end;

extern uint32_t __bss_start;
extern uint32_t __bss_end;

#endif
