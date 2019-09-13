/*
 * VentanaConfigurableFrame.cpp
 *
 *  Created on: 28 ago. 2019
 *      Author: iso9660
 */

#include <VentanaConfigurableFrame.h>
#include <ui.h>

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
	configurable_frame_raw_t *c = NULL;
	for (int i = 0; (c == NULL) && (i < config_frames_count); i++)
	{
		c = Same(frame_names[0], config_frames[i].name) ? &config_frames[i] : NULL;
	}
	if (c != NULL)
	{
		EntrySet(g_VentanaConfigurableFrameID, c->id);
	}
	else
	{
		EntrySet(g_VentanaConfigurableFrameID, "0x%02X", db->GetFrameByName(Str(frame_names[0]))->GetId());
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
		uint16_t id = EntryGetInt(g_VentanaConfigurableFrameID);

		// Name
		const gchar *name = gtk_combo_box_get_active_id(GTK_COMBO_BOX(g_VentanaConfigurableFrameName));

		// Compose the signal
		res = new ldfconfigurableframe(Str(name), id);
	}
	gtk_widget_hide(GTK_WIDGET(handle));

	return res;
}

void VentanaConfigurableFrame::OnVentanaConfigurableFrameAccept_clicked(GtkButton *button, gpointer user_data)
{
	VentanaConfigurableFrame *v = (VentanaConfigurableFrame *)user_data;

	// Check frame ID is unique
	const char *new_frame_name = gtk_combo_box_get_active_id(GTK_COMBO_BOX(v->g_VentanaConfigurableFrameName));
	int new_frame_id = EntryGetInt(v->g_VentanaConfigurableFrameID);
	for (int i = 0; i < v->config_frames_count; i++)
	{
		// Skip configurable frames with the same name than the one being edited
		if (Same(new_frame_name, v->config_frames[i].name))
			continue;

		// Skip configurable frames with different IDs
		if (!Same(new_frame_id, v->config_frames[i].id))
			continue;

		ShowErrorMessageBox(v->handle, "Configurable frame '%s' is already using the identifier '%s'.", v->config_frames[i].name, v->config_frames[i].id);
		return;
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
