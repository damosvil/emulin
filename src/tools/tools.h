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
#include "ldfcommon.h"

#define ARR_SIZE(A)				(sizeof(A) / sizeof(A[0]))
#define G_VAR(A)  				GObject *g_##A;
#define G_PIN(A) 				g_##A = gtk_builder_get_object(builder, #A)

#define G_CONNECT(A,B)			g_signal_connect(g_##A, #B, G_CALLBACK(On##A##_##B), this)
#define G_CONNECT_INSTXT(A, B)	g_signal_connect(g_##A, "insert-text", G_CALLBACK(EditableInsertValidator), B)

#define G_PAUSE_FUNC(A, B)		g_signal_handlers_block_matched(g_##A, G_SIGNAL_MATCH_FUNC, 0, 0, 0, (gpointer)B, 0)
#define G_PAUSE_DATA(A, B)		g_signal_handlers_block_matched(g_##A, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, B)

#define G_PLAY_FUNC(A, B)		g_signal_handlers_unblock_matched(g_##A, G_SIGNAL_MATCH_FUNC, 0, 0, 0, (gpointer)B, 0)
#define G_PLAY_DATA(A, B)		g_signal_handlers_unblock_matched(g_##A, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, B)

#define G_DISCONNECT_FUNC(A, B)	g_signal_handlers_disconnect_matched(g_##A, G_SIGNAL_MATCH_FUNC, 0, 0, 0, (gpointer)B, 0)
#define G_DISCONNECT_DATA(A, B)	g_signal_handlers_disconnect_matched(g_##A, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, B)


namespace tools
{

uint32_t GetStrIndexByID(const char **ids, uint32_t ids_count, const char *id);
bool RegExprCheck(const char *string, const char *pattern);
void EditableInsertValidator(GtkEditable *editable, gchar *new_text, gint new_text_length, gpointer position, gpointer user_data);
void EditableDeleteValidator (GtkEditable *editable, gint start_pos, gint end_pos, gpointer user_data);
void TreeViewPrepare(GObject *v, const char **columns);
const char *GetStrPrintf(const char *format, ...);
void ShowErrorMessageBox(GObject *parent, const char *format, ...);
bool ShowChooseMessageBox(GObject *parent, const char *format, ...);
void WidgetEnable(GObject *w, bool enable);
void WidgetShow(GObject *w, bool show);
void EntrySet(GObject *w, const uint8_t *v);
void EntrySet(GObject *w, const char *format, ...);
const char * EntryGetStr(GObject *w);
int EntryGetInt(GObject *w);
double EntryGetFloat(GObject *w);
int32_t MultiParseInt(const char *p);
bool Same(const char *a, const char *b);
bool Same(const uint8_t *a, const char *b);
bool Same(const char *a, const uint8_t *b);
bool Same(const uint8_t *a, const uint8_t *b);
bool Same(int a, const char *b);
bool Same(const char *a, int b);


}

#endif /* TOOLS_TOOLS_H_ */
