
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

#include "loader/strlen.h"

char* strrev( char* str ) {
  char* p1 = str;
  char* p2 = ( char* )( str + strlen( str ) - 1 );

  while ( p1 < p2 ) {
    char tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
    p1++;
    p2--;
  }

  return str;
}
