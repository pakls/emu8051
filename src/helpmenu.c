/*
 * helpmenu.c
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

#include <stdbool.h>

#if STDC_HEADERS
#  include <string.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif

#include <gtk/gtk.h>

#include "common.h"
#include "options.h"
#include "emugtk.h"
#include "messagebox.h"
#include "helpmenu.h"

#define PACKAGE_COPYRIGHT "(c) Hugo Villeneuve"

static void
HelpCommandsEvent(gchar *string)
{
	ShowMessage("Command Line Options", COMMAND_LINE_OPTIONS,
		    GTK_JUSTIFY_LEFT, MESSAGE_DIALOG_FIXED_FONT);
}

static void
HelpAboutEvent(GtkWidget *widget, gpointer data)
{
	const char *authors[] = {
		"Hugo Villeneuve <hugo@hugovil.com>",
		"Jonathan St-André",
		"Pascal Fecteau",
		"Jimmy Ringuette",
		NULL,
	};

	const char *license =
		"This program is free software; you can redistribute it"
		" and/or"
		" modify it under the terms of the GNU General Public License"
		" as published by the Free Software Foundation; either"
		" version 2 of the License, or (at your option) any later"
		" version.\n\n"
		"This program is distributed in the hope that it will be"
		" useful, but WITHOUT ANY WARRANTY; without even the implied"
		" warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR"
		" PURPOSE.  See the GNU General Public License for more"
		" details.\n\n"
		"You should have received a copy of the GNU General Public"
		" License along with this program. If not, see\n"
		"   <http://www.gnu.org/licenses/>";

	gtk_show_about_dialog(
		NULL,
		"name", PACKAGE_NAME,
		"title", "About Dialog",
		"version", PACKAGE_VERSION,
		"logo-icon-name", PACKAGE_TARNAME,
		"comments", get_package_description(),
		"authors", authors,
		"website", PACKAGE_URL,
		"copyright", PACKAGE_COPYRIGHT,
		"license", license,
		"wrap-license", true,
		NULL);
}

void
HelpAddMenu(GtkWidget *menu_bar)
{
	GtkWidget *item;
	GtkWidget *menu;

	menu = gtk_menu_new();

	/* Create the 'Help Command Line Options' item. */
	item = gtk_menu_item_new_with_label("Command Line Options");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	/* Attach the callback functions to the activate signal. */
	g_signal_connect(item, "activate", G_CALLBACK(HelpCommandsEvent), NULL);

	AddMenuSeparator(menu);

	/* Create the 'Help About' item. */
	item = gtk_menu_item_new_with_label("About " PACKAGE);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	/* Attach the callback functions to the activate signal. */
	g_signal_connect(item, "activate", G_CALLBACK(HelpAboutEvent), NULL);

	/* Adding submenu title. */
	item = gtk_menu_item_new_with_label("Help");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), menu);
	gtk_menu_shell_append((GtkMenuShell *) menu_bar, item);
}
