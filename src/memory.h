/*
 * memory.h
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef MEMORY_H
#define MEMORY_H 1

#include <sys/types.h>

#define PGM_MEM_SIZE 65536
#define INT_MEM_SIZE 256 /* IRAM = Direct addresses $00 to $7F
			  * SFR  = Direct addresses $80 to $FF */
#define EXT_MEM_SIZE 65536
#define EXT_MEM_DEFAULT_SIZE 1024

enum {
  PGM_MEM_ID,
  INT_MEM_ID,
  EXT_MEM_ID
};

void
memory_write8(int memory_id, unsigned long address, u_int8_t value);

u_int8_t
memory_read8(int memory_id, unsigned long address);

void
DumpMem(char *Address, char *Asize, int memory_id);

#endif /* MEMORY_H */
