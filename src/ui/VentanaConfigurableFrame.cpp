/*
 * VentanaConfigurableFrame.cpp
 *
 *  Created on: 28 ago. 2019
 *      Author: iso9660
 */

#include <VentanaConfigurableFrame.h>

namespace ui {

VentanaConfigurableFrame::VentanaConfigurableFrame(GtkBuilder *builder, ldf *db, char **frame_names, int frame_names_count, configurable_frame_raw_t *config_frames, int config_frames_count)
{
	// Store input info
	this->handle = gtk_builder_get_object(builder, "VentanaConfigurableFrame");
	this->db = db;
	this->config_frames = config_frames;
	this->config_frames_count = config_frames_count;

	// Pin widgets
	G_PIN(VentanaConfigurableFrameID);
	G_PIN(VentanaConfigurableFrameName);
	G_PIN(VentanaConfigurableFrameAccept);
	G_PIN(VentanaConfigurableFrameCancel);

	// Fill signals list with data
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(g_VentanaConfigurableFrameName));
	for (int ix = 0; ix < frame_names_count; ix++)
		gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(g_VentanaConfigurableFrameName), frame_names[ix], frame_names[ix]);
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaConfigurableFrameName), frame_names[0]);

	// Set ID
	for (int i = 0; i < config_frames_count; i++)
	{
		// Skip configurable frames with different name
		if (strcmp(frame_names[0], config_frames[i].name) != 0)
			continue;

		gtk_entry_set_text(GTK_ENTRY(g_VentanaConfigurableFrameID), config_frames[i].id);
		break;
	}

	// Connect text fields
	G_CONNECT_INSTXT(VentanaConfigurableFrameID, INT3_EXPR);

	// Connect nodes
	G_CONNECT(VentanaConfigurableFrameAccept, clicked);
	G_CONNECT(VentanaConfigurableFrameCancel, clicked);
}

VentanaConfigurableFrame::~VentanaConfigurableFrame()
{
	// Disconnect text fields
	G_DISCONNECT_FUNC(VentanaConfigurableFrameID, EditableInsertValidator);

	// Disconnect nodes
	G_DISCONNECT_DATA(VentanaConfigurableFrameAccept, this);
	G_DISCONNECT_DATA(VentanaConfigurableFrameCancel, this);
}

ldfconfigurableframe *VentanaConfigurableFrame::ShowModal(GObject *parent)
{
	ldfconfigurableframe *res = NULL;

	// Put this window always on top of parent
	gtk_window_set_transient_for(GTK_WINDOW(handle), GTK_WINDOW(parent));

	// Show dialog
	if (gtk_dialog_run(GTK_DIALOG(handle)))
	{
		// Offset
		uint16_t id = MultiParseInt(gtk_entry_get_text(GTK_ENTRY(g_VentanaConfigurableFrameID)));

		// Name
		const gchar *name = gtk_combo_box_get_active_id(GTK_COMBO_BOX(g_VentanaConfigurableFrameName));

		// Compose the signal
		res = new ldfconfigurableframe((uint8_t *)name, id);
	}
	gtk_widget_hide(GTK_WIDGET(handle));

	return res;
}

void VentanaConfigurableFrame::OnVentanaConfigurableFrameAccept_clicked(GtkButton *button, gpointer user_data)
{
	VentanaConfigurableFrame *v = (VentanaConfigurableFrame *)user_data;

	// Check frame ID is unique
	int new_frame_id = MultiParseInt(gtk_entry_get_text(GTK_ENTRY(v->g_VentanaConfigurableFrameID)));
	for (int i = 0; i < v->config_frames_count; i++)
	{
		// Skip configurable frames with different IDs
		if (new_frame_id == MultiParseInt(v->config_frames[i].id))
		{
			ShowErrorMessageBox(v->handle, "Signals shall have subscribers.");
			return;
		}
	}

	// Return true
	gtk_dialog_response(GTK_DIALOG(v->handle), true);
}

void VentanaConfigurableFrame::OnVentanaConfigurableFrameCancel_clicked(GtkButton *button, gpointer user_data)
{
	VentanaConfigurableFrame *v = (VentanaConfigurableFrame *)user_data;

	// Return false
	gtk_dialog_response(GTK_DIALOG(v->handle), false);
}


} /* namespace ui */
