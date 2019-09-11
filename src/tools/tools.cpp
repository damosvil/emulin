/*
 * tools.c
 *
 *  Created on: 24 jul. 2019
 *      Author: iso9660
 */

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
#include <gtk/gtk.h>
#include <stdarg.h>
#include "tools.h"

namespace tools
{

uint32_t GetStrIndexByID(const char **ids, uint32_t ids_count, const char *id)
{
	for (uint32_t i = 0; i < ids_count; i++)
		if (Same(id, ids[i]))
			return i;

	return ids_count;
}

bool RegExprCheck(const char *string, const char *pattern)
{
	regex_t regex;
	bool good = false;

	// Check expression
	if (regcomp(&regex, pattern, REG_EXTENDED | REG_ICASE) == 0)
	{
		// Check expression
		good = regexec(&regex, string, 0, NULL, 0) == 0;

		// Free reg expr resources
		regfree(&regex);
	}

	return good;
}

void EditableInsertValidator(GtkEditable *editable, gchar *new_text, gint new_text_length, gpointer position, gpointer user_data)
{
	uint32_t insert_position = gtk_editable_get_position(editable);

	// Store old expression
	const char *pattern = (const char *)user_data;
	const char *old_expr = gtk_entry_get_text(GTK_ENTRY(editable));

	// Compose new expression
	char *new_expr = (char *)malloc(strlen(old_expr) + new_text_length + 1);
	memcpy(new_expr, old_expr, insert_position);
	new_expr[insert_position] = 0;
	strcat(new_expr, new_text);
	strcat(new_expr, old_expr + insert_position);

	// Cancel action if necessary
	if (!RegExprCheck(new_expr, pattern))
	{
		g_signal_stop_emission_by_name(G_OBJECT(editable), "insert-text");
	}

	// Free old_expr copy
	free((void *)new_expr);
}

void EditableDeleteValidator(GtkEditable *editable, gint start_pos, gint end_pos, gpointer user_data)
{
	// Store old expression
	const char *pattern =  (const char *)user_data;
	const char *old_expr = gtk_entry_get_text(GTK_ENTRY(editable));

	// Compose new expression
	char *new_expr = (char *)malloc(strlen(old_expr) + 1 - (end_pos - start_pos));
	memcpy(new_expr, old_expr, start_pos);
	new_expr[start_pos] = 0;
	strcat(new_expr, old_expr + end_pos);

	// Cancel action if necessary
	if (!RegExprCheck(new_expr, pattern))
	{
		g_signal_stop_emission_by_name(G_OBJECT(editable), "delete-text");
	}

	// Free old_expr copy
	free((void *)new_expr);
}

static void TreeViewAddColumn(GtkTreeView *v, const gchar *title, gint column_index)
{
	GtkTreeViewColumn *c = gtk_tree_view_column_new();
	GtkCellRenderer *r = gtk_cell_renderer_text_new();

	gtk_tree_view_column_set_title(c, title);
	gtk_tree_view_append_column(v, c);
	gtk_tree_view_column_pack_start(c, r, TRUE);
	gtk_tree_view_column_add_attribute(c, r, "text", column_index);
}

static void TreeViewRemoveColumn(GtkTreeView *v, int ix)
{
	GtkTreeViewColumn *c = gtk_tree_view_get_column(v, ix);
	if (c != NULL) gtk_tree_view_remove_column(v, c);
}

void TreeViewPrepare(GObject *v, const char **columns)
{
	// Generic list store with 10 columns
	GtkListStore *s = gtk_list_store_new(10,
			G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
			G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

	// Remove columns
	for (int i = 0; columns[i] != NULL; i++)
	{
		TreeViewRemoveColumn(GTK_TREE_VIEW(v), 0);
	}

	// Add columns
	for (int i = 0; columns[i] != NULL; i++)
	{
		TreeViewAddColumn(GTK_TREE_VIEW(v), columns[i], i);
	}

	gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(v), GTK_TREE_VIEW_GRID_LINES_HORIZONTAL);

	// Set model and unmanage reference from this code
	gtk_tree_view_set_model(GTK_TREE_VIEW(v), GTK_TREE_MODEL(s));
	g_object_unref(s);
}

const char *GetStrPrintf(const char *format, ...)
{
	static char str[100000];
	static int ix = 0;
	va_list argptr;

	// Increase index
	ix = (ix < 90000) ? ix + 10000 : 0;

	// Print string
	va_start(argptr, format);
	vsprintf(str + ix, format, argptr);
	va_end(argptr);

	// return string
	return str + ix;
}

void ShowErrorMessageBox(GObject *parent, const char *format, ...)
{
	static char str[10000];
	va_list argptr;

	va_start(argptr, format);
	vsprintf(str, format, argptr);
	va_end(argptr);

	GtkWidget *d = gtk_message_dialog_new(
			GTK_WINDOW(parent),
			GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_MESSAGE_WARNING,
			GTK_BUTTONS_OK,
			str
			);

	gtk_window_set_title(GTK_WINDOW(d), "Error");
	gtk_dialog_run(GTK_DIALOG(d));
	gtk_widget_destroy(d);
}

bool ShowChooseMessageBox(GObject *parent, const char *format, ...)
{
	bool res;
	static char str[10000];
	va_list argptr;

	va_start(argptr, format);
	vsprintf(str, format, argptr);
	va_end(argptr);

	GtkWidget *d = gtk_message_dialog_new(
			GTK_WINDOW(parent),
			GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_MESSAGE_WARNING,
			GTK_BUTTONS_YES_NO,
			str
			);

	gtk_window_set_title(GTK_WINDOW(d), "Error");
	res = gtk_dialog_run(GTK_DIALOG(d)) == GTK_RESPONSE_YES;
	gtk_widget_destroy(d);

	return res;
}

void WidgetEnable(GObject *w, bool enable)
{
	gtk_widget_set_sensitive(GTK_WIDGET(w), enable);
}

void EntrySet(GObject *w, const uint8_t *v)
{
	gtk_entry_set_text(GTK_ENTRY(w), (gchar *)v);
}

void EntrySet(GObject *w, const char *format, ...)
{
	static char str[10000];
	va_list argptr;

	va_start(argptr, format);
	vsprintf(str, format, argptr);
	va_end(argptr);

	gtk_entry_set_text(GTK_ENTRY(w), str);
}

const char * EntryGetStr(GObject *w)
{
	return gtk_entry_get_text(GTK_ENTRY(w));
}

int EntryGetInt(GObject *w)
{
	return MultiParseInt(EntryGetStr(w));
}

double EntryGetFloat(GObject *w)
{
	return atof(EntryGetStr(w));
}

int32_t MultiParseInt(const char *p)
{
	return (p[1] == 'x' || p[1] == 'X') ? strtol(p, NULL, 16) : atoi(p);
}

bool Same(const char *a, const char *b)
{
	if (a == b)
		return true;
	if ((a == NULL) || (b == NULL))
		return false;

	return strcmp(a, b) == 0;
}

bool Same(const uint8_t *a, const char *b)
{
	return Same((const char *)a, (const char *)b);
}

bool Same(const char *a, const uint8_t *b)
{
	return Same((const char *)a, (const char *)b);
}

bool Same(const uint8_t *a, const uint8_t *b)
{
	return Same((const char *)a, (const char *)b);
}

bool Same(int a, const char *b)
{
	return a == MultiParseInt(b);
}

bool Same(const char *a, int b)
{
	return MultiParseInt(a) == b;
}


}
