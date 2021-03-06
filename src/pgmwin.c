/*
 * pgmwin.c
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
#include "memory.h"
#include "cpu8051.h"
#include "pgmwin.h"
#include "hexfile.h"

static GtkWidget *pgmlist;

#define LIST_VIEW_NAME "Program"
#define DATA_ROWS 100

enum
{
	COL_BREAKPT = 0,
	COL_ADDR,
	COL_B0,
	COL_B1,
	COL_B2,
	COL_INST,
	COL_ARGS,
	COL_COLOR,
	N_COLUMNS,
};

static char *col_names[N_COLUMNS] = {
	"BPT",
	"Address",
	"B0",
	"B1",
	"B2",
	"Mnemonic",
	"Arguments",
	"COLOR", /* Not displayed, used to set foreground color of cell. */
};

/* Creating a model */
static GtkListStore *
pgmwin_init_store(void)
{
	GtkTreeIter iter;
	int row;
	int col;
	GtkListStore *store;
	GType col_types[N_COLUMNS];

	/* No need for static array, all our columns are of the same type. */
	for (col = 0; col < N_COLUMNS; col++)
		col_types[col] = G_TYPE_STRING;

	store = gtk_list_store_newv(N_COLUMNS, col_types);

	/* Add rows. */
	for (row = 0; row < DATA_ROWS; row++) {
		gtk_list_store_append(store, &iter);

		/* Color first row in red (current instruction). */
		if (row == 0)
			gtk_list_store_set(store, &iter, COL_COLOR, "red", -1);
		else
			gtk_list_store_set(store, &iter, COL_COLOR, "black", -1);
	}

	return store;
}

static void
pgmwin_init_columns(void)
{
	int k;
	GtkCellRenderer *renderer;

	/* Create renderer */
	renderer = gtk_cell_renderer_text_new();

	/* Add columns, except for last one (COL_COLOR). */
	for (k = 0; k < COL_COLOR; k++) {
		GtkTreeViewColumn *col;

		/* Create tree view column */
		col = gtk_tree_view_column_new();
		gtk_tree_view_column_set_title(col, col_names[k]);
		gtk_tree_view_column_set_sizing(col,
						GTK_TREE_VIEW_COLUMN_AUTOSIZE);
		gtk_tree_view_append_column(GTK_TREE_VIEW(pgmlist), col);

		/* Pack cell renderer into column */
		gtk_tree_view_column_pack_start(col, renderer, TRUE);

		/* Establish connection between cell renderer and data store. */
		gtk_tree_view_column_set_attributes(col, renderer, "text", k,
						    "foreground", COL_COLOR,
						    NULL);
	}
}

/* Mouse button pressed in the window. */
static gint
pgmwin_sel_changed_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	GtkTreeSelection *selection;
	GtkTreeModel     *model;
	GtkTreeIter       iter;

	log_debug("pgmwin_sel_changed_event()");

	/* This will only work in single or browse selection mode! */
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(pgmlist));

	if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
		char *str_addr;
		int val;

		gtk_tree_model_get(model, &iter, COL_ADDR, &str_addr, -1);

		/* Convert hex address in ASCII to integer. */
		val = asciihex2int(str_addr);

		log_debug("  row address is: $%04X", val);

		ToggleBreakpoint(val);
		pgmwin_refresh();

		g_free(str_addr);
	} else {
		log_debug("  no row selected");
	}

	return FALSE;
}

GtkWidget *
pgmwin_init(void)
{
	GtkWidget *frame;
	GtkWidget *scrollwin;
	GtkListStore *store;
	GtkTreeSelection *selection;

	frame = gtk_frame_new(LIST_VIEW_NAME);

	scrollwin = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrollwin),
					    GTK_SHADOW_ETCHED_OUT);

	/* Automatically add scrollbars when necessary. */
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollwin),
				       GTK_POLICY_AUTOMATIC,
				       GTK_POLICY_AUTOMATIC);

	gtk_container_add(GTK_CONTAINER(frame), scrollwin);

	/* Creating a model */
	store = pgmwin_init_store();

	/* Creating the view component */
	pgmlist = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(pgmlist), TRUE);
	gtk_container_add(GTK_CONTAINER(scrollwin), pgmlist);

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(pgmlist));

	/* Only one row can be selected at a time. */
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_BROWSE);

	g_signal_connect(selection, "changed",
		G_CALLBACK(pgmwin_sel_changed_event), NULL);

	pgmwin_init_columns();

	/*
	 * The tree view has acquired its own reference to the model, so we can
	 * drop ours. That way the model will be freed automatically when the
	 * tree view is destroyed.
	 */
	g_object_unref(store);

	return frame;
}

/* Show disassembled program. */
void
pgmwin_refresh(void)
{
	int row;
	GtkListStore *store;
	unsigned int Address;

	Address = cpu8051.pc;

	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(pgmlist)));

	for (row = 0; row < DATA_ROWS; row++) {
		int valid;
		GtkTreeIter iter;
		char str[128];
		int k;
		int col_id;
		int InstSize;
		unsigned char OpCode;

		if (row == 0) {
			/* Get first row in list store */
			valid = gtk_tree_model_get_iter_first(
				GTK_TREE_MODEL(store), &iter);
		} else {
			/* Get next row in list store */
			valid = gtk_tree_model_iter_next(
				GTK_TREE_MODEL(store), &iter);
		}

		if (!valid) {
			printf("Invalid iter...\n");
			return;
		}

		if (Address > 0xFFFF) {
			/*
			 * Not the most elegant solution, but it works to not
			 * display instructions past last address, 0xFFFF.
			 */
			gtk_list_store_set(store, &iter,
					   COL_BREAKPT, NULL,
					   COL_ADDR, NULL,
					   COL_B0, NULL,
					   COL_B1, NULL,
					   COL_B2, NULL,
					   COL_INST, NULL,
					   COL_ARGS, NULL,
					   COL_COLOR, NULL,
					   -1);
		} else {
			/* Display breakpoints. */
			if (IsBreakpoint(Address))
				sprintf(str, "*");
			else
				str[0] = '\0';

			gtk_list_store_set(store, &iter, COL_BREAKPT, str, -1);

			/* Display base address. */
			int2asciihex(Address, str, 4);

			gtk_list_store_set(store, &iter, COL_ADDR, str, -1);

			OpCode = memory_read8(PGM_MEM_ID, Address);
			InstSize = cpu8051_get_instruction_size(OpCode);

			/* Display instruction hex bytes. */
			for (k = 0, col_id = COL_B0; k < 3; k++, col_id++) {
				if (k < InstSize)
					int2asciihex(memory_read8(PGM_MEM_ID,
								  Address + k),
						     str, 2);
				else
					str[0] = '\0';

				gtk_list_store_set(store, &iter, col_id, str, -1);
			}

			/* Display instruction menmonic. */
			cpu8051_disasm_mnemonic(OpCode, str);
			gtk_list_store_set(store, &iter, COL_INST, str, -1);

			/* Display instruction arguments (if applicable). */
			str[0] = '\0';
			cpu8051_disasm_args(Address, str);
			gtk_list_store_set(store, &iter, COL_ARGS, str, -1);

			Address += InstSize;
		}
	}
}
