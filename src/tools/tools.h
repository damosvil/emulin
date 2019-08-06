/*
 * tools.h
 *
 *  Created on: 24 jul. 2019
 *      Author: iso9660
 */

#ifndef TOOLS_TOOLS_H_
#define TOOLS_TOOLS_H_

#include <stdint.h>
#include <stdbool.h>
#include <gtk/gtk.h>


#define ARR_SIZE(A)			(sizeof(A) / sizeof(A[0]))
#define G_VAR(A)  			GObject *g_##A;
#define G_PIN(A) 			g_##A = gtk_builder_get_object(builder, #A)
#define G_CONNECT(A,B,C)	g_signal_connect(g_##A, C, G_CALLBACK(On##A##B), this)
#define G_PAUSE(A)			g_signal_handlers_block_matched(g_##A, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, this)
#define G_PLAY(A)			g_signal_handlers_unblock_matched(g_##A, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, this)


#define INT_EXPR			(gpointer)"^[0-9]{1,5}$"
#define NAME_EXPR			(gpointer)"^[A-Za-z]{1}([A-Za-z_0-9])+$"
#define SFLOAT_EXPR			(gpointer)"^[0-9]{1,2}[.]{0,1}[0-9]{0,3}$"


#ifdef __cplusplus
extern "C" {
#endif

uint32_t GetStrIndexByID(const char **ids, uint32_t ids_count, const char *id);
bool RegExprCheck(const char *string, const char *pattern);
void EditableInsertValidator(GtkEditable *editable, gchar *new_text, gint new_text_length, gpointer position, gpointer user_data);
void EditableDeleteValidator (GtkEditable *editable, gint start_pos, gint end_pos, gpointer user_data);


#ifdef __cplusplus
}
#endif

#endif /* TOOLS_TOOLS_H_ */
