/*
 * VentanaSelectSignal.cpp
 *
 *  Created on: 13 ago. 2019
 *      Author: iso9660
 */

#include <VentanaSelectSignal.h>

namespace ui {

VentanaSelectSignal::VentanaSelectSignal(GtkBuilder *builder, const char *signal_name, const char **signal_names, int signal_names_count)
{
	// Store input info
	this->handle = gtk_builder_get_object(builder, "VentanaSelectSignal");

	// Pin widgets
	G_PIN(VentanaSelectSignalName);
	G_PIN(VentanaSelectSignalAccept);
	G_PIN(VentanaSelectSignalCancel);

	// Fill lin signals list with data
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(g_VentanaSelectSignalName));
	for (int ix = 0; ix < signal_names_count; ix++)
		gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(g_VentanaSelectSignalName), (char *)signal_names[ix], (char *)signal_names[ix]);

	// Select lin signal
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaSelectSignalName), (signal_name != NULL) ? signal_name : signal_names[0]);

	// Connect signals
	G_CONNECT(VentanaSelectSignalAccept, clicked);
	G_CONNECT(VentanaSelectSignalCancel, clicked);
}

VentanaSelectSignal::~VentanaSelectSignal()
{
	// Disconnect signals
	G_DISCONNECT_DATA(VentanaSelectSignalAccept, this);
	G_DISCONNECT_DATA(VentanaSelectSignalCancel, this);
}

void VentanaSelectSignal::OnVentanaSelectSignalAccept_clicked(GtkButton *button, gpointer user_data)
{
	VentanaSelectSignal *v = (VentanaSelectSignal *)user_data;

	// Return false
	gtk_dialog_response(GTK_DIALOG(v->handle), true);
}

void VentanaSelectSignal::OnVentanaSelectSignalCancel_clicked(GtkButton *button, gpointer user_data)
{
	VentanaSelectSignal *v = (VentanaSelectSignal *)user_data;

	// Return false
	gtk_dialog_response(GTK_DIALOG(v->handle), false);
}

const char *VentanaSelectSignal::ShowModal(GObject *parent)
{
	const char *res = NULL;

	// Put this window always on top of parent
	gtk_window_set_transient_for(GTK_WINDOW(handle), GTK_WINDOW(parent));

	// Show dialog
	if (gtk_dialog_run(GTK_DIALOG(handle)))
	{
		// Name
		res = gtk_combo_box_get_active_id(GTK_COMBO_BOX(g_VentanaSelectSignalName));
	}
	gtk_widget_hide(GTK_WIDGET(handle));

	return res;
}


} /* namespace ui */
