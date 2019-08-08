/*
 * VentanaNodoEsclavo.cpp
 *
 *  Created on: 7 ago. 2019
 *      Author: iso9660
 */

#include <VentanaNodoEsclavo.h>

namespace ui {

VentanaNodoEsclavo::VentanaNodoEsclavo(GtkBuilder *builder, ldf *db, uint8_t *slave_name)
{
	// Store input info
	this->db = db;
	this->slave_name = slave_name;
	this->handle = gtk_builder_get_object(builder, "VentanaNodoEsclavo");

	// Pin widgets
	G_PIN(Name);
	G_PIN(ProtocolVersion);
	G_PIN(InitialNAD);
	G_PIN(ConfiguredNAD);
	G_PIN(SupplierID);
	G_PIN(FunctionID);
	G_PIN(Variant);
	G_PIN(ResponseErrorSignal);
	G_PIN(P2_min);
	G_PIN(ST_min);	// Show dialog

	G_PIN(N_As_timeout);
	G_PIN(N_Cr_timeout);
	G_PIN(ConfigFrameList);
	G_PIN(ConfigFrameNew);
	G_PIN(ConfigFrameEdit);
	G_PIN(ConfigFrameDelete);
	G_PIN(Accept);
	G_PIN(Cancel);

	// Connect button widgets
	G_CONNECT(ConfigFrameNew, Clicked, "clicked");
	G_CONNECT(ConfigFrameEdit, Clicked, "clicked");
	G_CONNECT(ConfigFrameDelete, Clicked, "clicked");
	G_CONNECT(Accept, Clicked, "clicked");
	G_CONNECT(Cancel, Clicked, "clicked");
}

VentanaNodoEsclavo::~VentanaNodoEsclavo()
{
	// Free resources
}

ldfnodeattributes *VentanaNodoEsclavo::ShowModal(GObject *parent)
{
	ldfnodeattributes *res = NULL;

	// Put this window on top of parent
	gtk_window_set_transient_for(GTK_WINDOW(handle), GTK_WINDOW(parent));

	// Show dialog
	if (gtk_dialog_run(GTK_DIALOG(handle)))
	{
		res = new ldfnodeattributes((uint8_t *)"");
	}
	gtk_widget_hide(GTK_WIDGET(handle));

	return res;
}

void VentanaNodoEsclavo::OnConfigFrameNewClicked(GtkButton *button, gpointer user_data)
{
	VentanaNodoEsclavo *v = (VentanaNodoEsclavo *)user_data;

}

void VentanaNodoEsclavo::OnConfigFrameEditClicked(GtkButton *button, gpointer user_data)
{
	VentanaNodoEsclavo *v = (VentanaNodoEsclavo *)user_data;

}

void VentanaNodoEsclavo::OnConfigFrameDeleteClicked(GtkButton *button, gpointer user_data)
{
	VentanaNodoEsclavo *v = (VentanaNodoEsclavo *)user_data;

}

void VentanaNodoEsclavo::OnAcceptClicked(GtkButton *button, gpointer user_data)
{
	VentanaNodoEsclavo *v = (VentanaNodoEsclavo *)user_data;

	// Return true
	gtk_dialog_response(GTK_DIALOG(v->handle), true);
}

void VentanaNodoEsclavo::OnCancelClicked(GtkButton *button, gpointer user_data)
{
	VentanaNodoEsclavo *v = (VentanaNodoEsclavo *)user_data;

	// Return false
	gtk_dialog_response(GTK_DIALOG(v->handle), false);
}


} /* namespace ui */
