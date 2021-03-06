/*
 * viewmenu.c
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

#include <gtk/gtk.h>

#include "common.h"
#include "emugtk.h" /* For AddMenuSeparator() function. */
#include "messagebox.h"
#include "viewmenu.h"
#include "app-config.h"

extern struct app_config_t *cfg;

void toggle_layout(GtkWidget *widget, gpointer data)
{
	int id;

        id = GPOINTER_TO_UINT(data);

	if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
		log_info("  Switching to layout %d", id);
		cfg->layout = id;
		emugtk_restart_gui();
	}
}

void toggle_bits_per_row(GtkWidget *widget, gpointer data)
{
	int bits_per_row;

        bits_per_row = GPOINTER_TO_UINT(data);

	if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
		log_info("  Bits per row = %d", bits_per_row);
		cfg->bits_per_row = bits_per_row;
		emugtk_restart_gui();
	}
}

void toggle_int_memory(GtkWidget *widget, gpointer data)
{
	if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
		log_info("  View internal memory");
		cfg->view_int_memory = 1;
	} else {
		cfg->view_int_memory = 0;
	}

	emugtk_restart_gui();
}

void toggle_sfr_memory(GtkWidget *widget, gpointer data)
{
	if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
		log_info("  View SFR memory");
		cfg->view_sfr_memory = 1;
	} else {
		cfg->view_sfr_memory = 0;
	}

	emugtk_restart_gui();
}

void toggle_ext_memory(GtkWidget *widget, gpointer data)
{
	if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
		log_info("  View external memory");
		cfg->view_ext_memory = 1;
	} else {
		cfg->view_ext_memory = 0;
	}

	emugtk_restart_gui();
}

void
view_add_layout_submenu(GtkWidget *parent)
{
	GtkWidget *submenu;
	GtkWidget *layout;
	GtkWidget *layout1;
	GtkWidget *layout2;
	GSList *group = NULL;

	submenu = gtk_menu_new();

	layout  = gtk_menu_item_new_with_label("Layout");

	layout1 = gtk_radio_menu_item_new_with_label(group, "Layout1");
	group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(layout1));
	layout2 = gtk_radio_menu_item_new_with_label(group, "Layout2");

	if (cfg->layout == UI_LAYOUT1)
		gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(layout1), TRUE);
	else
		gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(layout2), TRUE);

	g_signal_connect(G_OBJECT(layout1), "activate",
			 G_CALLBACK(toggle_layout), (gpointer) UI_LAYOUT1);
	g_signal_connect(G_OBJECT(layout2), "activate",
			 G_CALLBACK(toggle_layout), (gpointer) UI_LAYOUT2);

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(layout), submenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), layout1);
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), layout2);
	gtk_menu_shell_append(GTK_MENU_SHELL(parent), layout);
}

void
view_add_bits_per_row_submenu(GtkWidget *parent)
{
	GtkWidget *submenu;
	GtkWidget *item;
	GtkWidget *item1;
	GtkWidget *item2;
	GSList *group = NULL;

	submenu = gtk_menu_new();

	item  = gtk_menu_item_new_with_label("Bits per row");

	item1 = gtk_radio_menu_item_new_with_label(group, "8");
	group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(item1));
	item2 = gtk_radio_menu_item_new_with_label(group, "16");

	if (cfg->bits_per_row == 8)
		gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item1), TRUE);
	else
		gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item2), TRUE);

	g_signal_connect(G_OBJECT(item1), "activate",
			 G_CALLBACK(toggle_bits_per_row), (gpointer) 8);
	g_signal_connect(G_OBJECT(item2), "activate",
			 G_CALLBACK(toggle_bits_per_row), (gpointer) 16);

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), submenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), item1);
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), item2);
	gtk_menu_shell_append(GTK_MENU_SHELL(parent), item);
}

void
ViewAddMenu(GtkWidget *menu_bar)
{
	GtkWidget *item;
	GtkWidget *menu;
	GtkWidget *view;

	menu = gtk_menu_new();

	view = gtk_menu_item_new_with_label("View");

	item = gtk_check_menu_item_new_with_label("Internal Memory");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item),
				       cfg->view_int_memory);
	g_signal_connect(G_OBJECT(item), "activate",
			 G_CALLBACK(toggle_int_memory), NULL);

	item = gtk_check_menu_item_new_with_label("SFR Memory");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item),
				       cfg->view_sfr_memory);
	g_signal_connect(G_OBJECT(item), "activate",
			 G_CALLBACK(toggle_sfr_memory), NULL);

	item = gtk_check_menu_item_new_with_label("External Memory");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item),
				       cfg->view_ext_memory);
	g_signal_connect(G_OBJECT(item), "activate",
			 G_CALLBACK(toggle_ext_memory), NULL);

	AddMenuSeparator(menu);

	/* Add layout submenu */
	view_add_layout_submenu(menu);

	AddMenuSeparator(menu);

	/* Add bits per row submenu */
	view_add_bits_per_row_submenu(menu);

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view);
}
