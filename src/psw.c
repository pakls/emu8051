/*
 * psw.c
 *
 * Copyright (C) 2013 Hugo Villeneuve <hugo@hugovil.com>
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

#include "common.h"
#include "reg8051.h"
#include "memory.h"

/* Returns 0 or 1 */
int
psw_read_bit(int bit)
{
	return (memory_read8(INT_MEM_ID, _PSW_) >> bit) & 0x01;
}

void
psw_write_bit(int bit, int val)
{
	u_int8_t psw = memory_read8(INT_MEM_ID, _PSW_);

	if (val)
		psw |= (1 << bit);  /* Set */
	else
		psw &= ~(1 << bit); /* Clear */

	memory_write8(INT_MEM_ID, _PSW_, psw); /* Save updated value */
}

/* Returns 0 or 1 */
int
psw_read_cy(void)
{
	return psw_read_bit(PSW_BIT_CY);
}

void
psw_write_cy(int cy)
{
	psw_write_bit(PSW_BIT_CY, cy);
}

void
psw_set_cy(void)
{
	psw_write_bit(PSW_BIT_CY, 1);
}

void
psw_clr_cy(void)
{
	psw_write_bit(PSW_BIT_CY, 0);
}

/* Returns 0 or 1 */
int
psw_read_ac(void)
{
	return psw_read_bit(PSW_BIT_AC);
}

void
psw_write_ac(int ac)
{
	psw_write_bit(PSW_BIT_AC, ac);
}

void
psw_set_ac(void)
{
	psw_write_bit(PSW_BIT_AC, 1);
}

void
psw_clr_ac(void)
{
	psw_write_bit(PSW_BIT_AC, 0);
}

/* Returns 0 or 1 */
int
psw_read_ov(void)
{
	return psw_read_bit(PSW_BIT_OV);
}

void
psw_write_ov(int ov)
{
	psw_write_bit(PSW_BIT_OV, ov);
}

void
psw_set_ov(void)
{
	psw_write_bit(PSW_BIT_OV, 1);
}

void
psw_clr_ov(void)
{
	psw_write_bit(PSW_BIT_OV, 0);
}

/*
 * Compute parity of bits in accumulator:
 *   parity = 0: even number of ones in accumulator
 *   parity = 1: odd  number of ones in accumulator
 */
void
psw_compute_parity_bit(void)
{
	int parity = 0;
	uint8_t acc = memory_read8(INT_MEM_ID, _ACC_);

	while (acc) {
		parity = !parity;
		acc = acc & (acc - 1);
	}

	psw_write_bit(PSW_BIT_P, parity);
}
