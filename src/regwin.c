/*
 * regwin.cpp
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

#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include <stdio.h>

#include "common.h"
#include "reg8051.h"
#include "cpu8051.h"
#include "regwin.h"

static GtkWidget *regclist;

GtkWidget *
regwin_init(int width, int height)
{
	int i;
	GtkWidget *fixed_frame;
	PangoFontDescription *pango_font;
	char *regdummy[] = { 0 };

	fixed_frame = gtk_frame_new(0);
	gtk_frame_set_shadow_type(GTK_FRAME(fixed_frame),
				  GTK_SHADOW_ETCHED_OUT);
	gtk_widget_set_usize(GTK_WIDGET(fixed_frame), width, height);

	regclist = gtk_clist_new(1);
	gtk_clist_set_selection_mode(GTK_CLIST(regclist), GTK_SELECTION_SINGLE);
	gtk_widget_set_usize(GTK_WIDGET(regclist), width, height);
	gtk_clist_set_column_justification(GTK_CLIST(regclist), 0,
					   GTK_JUSTIFY_LEFT);
	gtk_clist_set_column_width(GTK_CLIST(regclist), 0, width);


	pango_font = pango_font_description_from_string(FIXED_FONT);
	gtk_widget_modify_font(regclist, pango_font);

	for (i = 0; i < 24; i++)
		gtk_clist_append(GTK_CLIST(regclist), regdummy);

	gtk_container_add(GTK_CONTAINER(fixed_frame), regclist);

	return fixed_frame;
}


/* Show registers. */
void
regwin_Show(void)
{
	char TextTmp[255];
	int row = 0;
	unsigned char PSW = cpu8051_ReadD(_PSW_);
	unsigned char Rbank;

	gtk_clist_freeze(GTK_CLIST(regclist));

	/* Main registers */
	sprintf(TextTmp , "PC   = %.4X", cpu8051.pc);
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);
	sprintf(TextTmp , "SP   =   %.2X", cpu8051_ReadD(_SP_));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);
	sprintf(TextTmp , "A    =   %.2X", cpu8051_ReadD(_ACC_));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);
	sprintf(TextTmp , "B    =   %.2X", cpu8051_ReadD(_B_));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);
	sprintf(TextTmp , "DPTR = %.4X", (cpu8051_ReadD(_DPTRHIGH_) << 8) +
		cpu8051_ReadD(_DPTRLOW_));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);

	/* Program Status Word */
	sprintf(TextTmp, "PSW  =   %.2X", PSW);
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);

	/* Ports registers */
	sprintf(TextTmp , "P0   =   %.2X", cpu8051_ReadD(_P0_));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);
	sprintf(TextTmp , "P1   =   %.2X", cpu8051_ReadD(_P1_));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);
	sprintf(TextTmp , "P2   =   %.2X", cpu8051_ReadD(_P2_));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);
	sprintf(TextTmp , "P3   =   %.2X", cpu8051_ReadD(_P3_));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);

	/* Misc Registers */
	sprintf(TextTmp , "TCON =   %.2X", cpu8051_ReadD(_TCON_));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);
	sprintf(TextTmp , "TMOD =   %.2X", cpu8051_ReadD(_TMOD_));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);
	sprintf(TextTmp , "SCON =   %.2X", cpu8051_ReadD(_SCON_));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);
	sprintf(TextTmp , "IE   =   %.2X", cpu8051_ReadD(_IE_));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);
	sprintf(TextTmp , "IP   =   %.2X", cpu8051_ReadD(_IP_));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);

	/* R0-R7 Registers in current Bank */
	Rbank = cpu8051_ReadD(_PSW_) & 0x18;
	sprintf(TextTmp , "Bank =   %.2X", Rbank);
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);
	sprintf(TextTmp , "R0   =   %.2X", cpu8051_ReadD(_R0_ + Rbank));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);
	sprintf(TextTmp , "R1   =   %.2X", cpu8051_ReadD(_R1_ + Rbank));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);
	sprintf(TextTmp , "R2   =   %.2X", cpu8051_ReadD(_R2_ + Rbank));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);
	sprintf(TextTmp , "R3   =   %.2X", cpu8051_ReadD(_R3_ + Rbank));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);
	sprintf(TextTmp , "R4   =   %.2X", cpu8051_ReadD(_R4_ + Rbank));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);
	sprintf(TextTmp , "R5   =   %.2X", cpu8051_ReadD(_R5_ + Rbank));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);
	sprintf(TextTmp , "R6   =   %.2X", cpu8051_ReadD(_R6_ + Rbank));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);
	sprintf(TextTmp , "R7   =   %.2X", cpu8051_ReadD(_R7_ + Rbank));
	gtk_clist_set_text(GTK_CLIST(regclist), row++, 0, TextTmp);

	gtk_clist_select_row(GTK_CLIST(regclist), 0, 0);
	gtk_clist_thaw(GTK_CLIST(regclist));
}
