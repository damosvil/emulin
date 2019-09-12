/*
 * VentanaFrameSignal.cpp
 *
 *  Created on: 22 ago. 2019
 *      Author: iso9660
 */

#include <VentanaFrameSignal.h>

namespace ui {

VentanaFrameSignal::VentanaFrameSignal(
		GtkBuilder *builder, ldf *db,
		char **signal_names, int signal_names_count,
		frame_signals_raw_t *frame_signals, int frame_signals_count)
{
	// Store input info
	this->handle = gtk_builder_get_object(builder, "VentanaFrameSignal");
	this->db = db;
	this->frame_signals = frame_signals;
	this->frame_signals_count = frame_signals_count;

	// Pin widgets
	G_PIN(VentanaFrameSignalOffset);
	G_PIN(VentanaFrameSignalName);
	G_PIN(VentanaFrameSignalSize);
	G_PIN(VentanaFrameSignalAccept);
	G_PIN(VentanaFrameSignalCancel);

	// Fill signals list with data
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(g_VentanaFrameSignalName));
	for (int ix = 0; ix < signal_names_count; ix++)
		gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(g_VentanaFrameSignalName), signal_names[ix], signal_names[ix]);

	// Set offset
	int frame_signal_ix = -1;
	for (int i = 0; (frame_signal_ix == -1) && (i < frame_signals_count); i++)
		frame_signal_ix = Same(signal_names[0], frame_signals[i].name) ? i : -1;
	if (frame_signal_ix == -1)
	{
		int offset = 0;
		for (int i = 0; i < frame_signals_count; i++)
		{
			int q = MultiParseInt(frame_signals[i].offset);
			q += db->GetSignalByName(Str(frame_signals[i].name))->GetBitSize();
			offset = (q > offset) ? q : offset;
		}
		EntrySet(g_VentanaFrameSignalOffset, "%d", offset);
	}
	else
	{
		EntrySet(g_VentanaFrameSignalOffset, frame_signals[frame_signal_ix].offset);
	}

	// Select signal
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaFrameSignalName), signal_names[0]);

	// Set bit size
	EntrySet(g_VentanaFrameSignalSize, "%d", db->GetSignalByName(Str(signal_names[0]))->GetBitSize());

	// Connect text fields
	G_CONNECT_INSTXT(VentanaFrameSignalOffset, INT3_EXPR);

	// Connect list
	G_CONNECT(VentanaFrameSignalName, changed);

	// Connect nodes
	G_CONNECT(VentanaFrameSignalAccept, clicked);
	G_CONNECT(VentanaFrameSignalCancel, clicked);
}

VentanaFrameSignal::~VentanaFrameSignal()
{
	// Disconnect text fields
	G_DISCONNECT_FUNC(VentanaFrameSignalOffset, INT3_EXPR);

	// Disconnect list
	G_DISCONNECT_DATA(VentanaFrameSignalName, this);

	// Disconnect nodes
	G_DISCONNECT_DATA(VentanaFrameSignalAccept, this);
	G_DISCONNECT_DATA(VentanaFrameSignalCancel, this);
}

void VentanaFrameSignal::OnVentanaFrameSignalName_changed(GtkComboBoxText *widget, gpointer user_data)
{
	VentanaFrameSignal *v = (VentanaFrameSignal *)user_data;
	const char *new_signal_name = gtk_combo_box_get_active_id(GTK_COMBO_BOX(v->g_VentanaFrameSignalName));

	// Set bit size
	EntrySet(v->g_VentanaFrameSignalSize, "%d", v->db->GetSignalByName(Str(new_signal_name))->GetBitSize());
}

void VentanaFrameSignal::OnVentanaFrameSignalAccept_clicked(GtkButton *button, gpointer user_data)
{
	VentanaFrameSignal *v = (VentanaFrameSignal *)user_data;

	// Check signal collision
	const char *new_signal_name = gtk_combo_box_get_active_id(GTK_COMBO_BOX(v->g_VentanaFrameSignalName));
	int new_signal_offset = EntryGetInt(v->g_VentanaFrameSignalOffset);
	int new_signal_size = v->db->GetSignalByName(Str(new_signal_name))->GetBitSize();
	for (int i = 0; i < v->frame_signals_count; i++)
	{
		// Skip editing signal
		if (Same(new_signal_name, v->frame_signals[i].name))
			continue;

		// Check collision
		int signal_offset = MultiParseInt(v->frame_signals[i].offset);
		int signal_size = v->db->GetSignalByName(Str(v->frame_signals[i].name))->GetBitSize();

		int a = new_signal_offset;
		int A = new_signal_offset + new_signal_size;
		int b = signal_offset;
		int B = signal_offset + signal_size;

		if ((a >= b && a < B) || (A > b && A <= B) || (b >= a && b < A) || (B > a && B <= A))
		{
			ShowErrorMessageBox(v->handle, "Signal '%s' collides with '%s'.", new_signal_name, v->frame_signals[i].name);
			return;
		}
	}

	// Return false
	gtk_dialog_response(GTK_DIALOG(v->handle), true);
}

void VentanaFrameSignal::OnVentanaFrameSignalCancel_clicked(GtkButton *button, gpointer user_data)
{
	VentanaFrameSignal *v = (VentanaFrameSignal *)user_data;

	// Return false
	gtk_dialog_response(GTK_DIALOG(v->handle), false);
}

ldfframesignal *VentanaFrameSignal::ShowModal(GObject *parent)
{
	ldfframesignal *res = NULL;

	// Put this window always on top of parent
	gtk_window_set_transient_for(GTK_WINDOW(handle), GTK_WINDOW(parent));

	// Show dialog
	if (gtk_dialog_run(GTK_DIALOG(handle)))
	{
		// Offset
		uint16_t offset = EntryGetInt(g_VentanaFrameSignalOffset);

		// Name
		const gchar *name = gtk_combo_box_get_active_id(GTK_COMBO_BOX(g_VentanaFrameSignalName));

		// Compose the signal
		res = new ldfframesignal(Str(name), offset);
	}
	gtk_widget_hide(GTK_WIDGET(handle));

	return res;
}


} /* namespace ui */
