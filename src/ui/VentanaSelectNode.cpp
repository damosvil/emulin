/*
 * VentanaSelectNode.cpp
 *
 *  Created on: 13 ago. 2019
 *      Author: iso9660
 */

#include <VentanaSelectNode.h>

namespace ui {

VentanaSelectNode::VentanaSelectNode(GtkBuilder *builder, const char **node_names, int node_names_count)
{
	// Store input info
	this->handle = gtk_builder_get_object(builder, "VentanaSelectNode");

	// Pin widgets
	G_PIN(VentanaSelectNodeName);
	G_PIN(VentanaSelectNodeAccept);
	G_PIN(VentanaSelectNodeCancel);

	// Fill lin nodes list with data and select lin node
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(g_VentanaSelectNodeName));
	for (int ix = 0; ix < node_names_count; ix++)
		gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(g_VentanaSelectNodeName), (char *)node_names[ix], (char *)node_names[ix]);
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaSelectNodeName), node_names[0]);

	// Connect nodes
	G_CONNECT(VentanaSelectNodeAccept, clicked);
	G_CONNECT(VentanaSelectNodeCancel, clicked);
}

VentanaSelectNode::~VentanaSelectNode()
{
	// Disconnect signals
	G_DISCONNECT_DATA(VentanaSelectNodeAccept, this);
	G_DISCONNECT_DATA(VentanaSelectNodeCancel, this);
}

void VentanaSelectNode::OnVentanaSelectNodeAccept_clicked(GtkButton *button, gpointer user_data)
{
	VentanaSelectNode *v = (VentanaSelectNode *)user_data;

	// Return false
	gtk_dialog_response(GTK_DIALOG(v->handle), true);
}

void VentanaSelectNode::OnVentanaSelectNodeCancel_clicked(GtkButton *button, gpointer user_data)
{
	VentanaSelectNode *v = (VentanaSelectNode *)user_data;

	// Return false
	gtk_dialog_response(GTK_DIALOG(v->handle), false);
}

const char *VentanaSelectNode::ShowModal(GObject *parent)
{
	const char *res = NULL;

	// Put this window always on top of parent
	gtk_window_set_transient_for(GTK_WINDOW(handle), GTK_WINDOW(parent));

	// Show dialog
	if (gtk_dialog_run(GTK_DIALOG(handle)))
	{
		// Name
		res = gtk_combo_box_get_active_id(GTK_COMBO_BOX(g_VentanaSelectNodeName));
	}
	gtk_widget_hide(GTK_WIDGET(handle));

	return res;
}


} /* namespace ui */
