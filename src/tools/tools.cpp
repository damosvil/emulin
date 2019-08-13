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
	uint32_t ix = 0;

	for (ix = 0; ix < ids_count; ix++)
	{
		if (id == NULL) break;
		if (ids[ix] == NULL) continue;
		if (strcmp(id, ids[ix]) == 0) break;
	}

	return ix;
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

void TreeViewAddColumn(GtkTreeView *v, const gchar *title, gint column_index)
{
	GtkTreeViewColumn *c = gtk_tree_view_column_new();
	GtkCellRenderer *r = gtk_cell_renderer_text_new();

	gtk_tree_view_column_set_title(c, title);
	gtk_tree_view_append_column(v, c);
	gtk_tree_view_column_pack_start(c, r, TRUE);
	gtk_tree_view_column_add_attribute(c, r, "text", column_index);
}

void TreeViewRemoveColumn(GtkTreeView *v, int ix)
{
	GtkTreeViewColumn *c = gtk_tree_view_get_column(v, ix);
	if (c != NULL) gtk_tree_view_remove_column(v, c);
}

const char *GetStrPrintf(const char *format, ...)
{
	static char str[10000];
	va_list argptr;

	va_start(argptr, format);
	vsprintf(str, format, argptr);
	va_end(argptr);

	return str;
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

int32_t MultiParseInt(const char *p)
{
	return (p[1] == 'x' || p[1] == 'X') ? strtoul(p, NULL, 16) : atoi(p);
}

}
