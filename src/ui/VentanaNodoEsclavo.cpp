/*
 * VentanaNodoEsclavo.cpp
 *
 *  Created on: 7 ago. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <VentanaNodoEsclavo.h>
#include "tools.h"

using namespace tools;

namespace ui {

VentanaNodoEsclavo::VentanaNodoEsclavo(GtkBuilder *builder, ldf *db, uint8_t *slave_name)
{
	// Store input info
	this->db = db;
	this->slave_name = slave_name;
	this->handle = gtk_builder_get_object(builder, "VentanaNodoEsclavo");

	// Pin widgets
	G_PIN(VentanaNodoEsclavoName);
	G_PIN(VentanaNodoEsclavoProtocolVersion);
	G_PIN(VentanaNodoEsclavoInitialNAD);
	G_PIN(VentanaNodoEsclavoConfiguredNAD);
	G_PIN(VentanaNodoEsclavoSupplierID);
	G_PIN(VentanaNodoEsclavoFunctionID);
	G_PIN(VentanaNodoEsclavoVariant);
	G_PIN(VentanaNodoEsclavoResponseErrorSignal);
	G_PIN(VentanaNodoEsclavoP2_min);
	G_PIN(VentanaNodoEsclavoST_min);
	G_PIN(VentanaNodoEsclavoN_As_timeout);
	G_PIN(VentanaNodoEsclavoN_Cr_timeout);
	G_PIN(VentanaNodoEsclavoConfigFrameList);
	G_PIN(VentanaNodoEsclavoConfigFrameNew);
	G_PIN(VentanaNodoEsclavoConfigFrameEdit);
	G_PIN(VentanaNodoEsclavoConfigFrameDelete);
	G_PIN(VentanaNodoEsclavoAccept);
	G_PIN(VentanaNodoEsclavoCancel);

	// Fill dialog fields with data
	ldfnodeattributes *a = (slave_name != NULL) ? db->GetSlaveNodeAttributes(slave_name) : NULL;
	if (a != NULL)
	{
		// Name	G_PIN(VentanaNodoEsclavoProtocolVersion);
		G_PIN(VentanaNodoEsclavoInitialNAD);
		G_PIN(VentanaNodoEsclavoConfiguredNAD);
		G_PIN(VentanaNodoEsclavoSupplierID);
		G_PIN(VentanaNodoEsclavoFunctionID);
		G_PIN(VentanaNodoEsclavoVariant);
		G_PIN(VentanaNodoEsclavoResponseErrorSignal);
		G_PIN(VentanaNodoEsclavoP2_min);
		G_PIN(VentanaNodoEsclavoST_min);
		G_PIN(VentanaNodoEsclavoN_As_timeout);
		G_PIN(VentanaNodoEsclavoN_Cr_timeout);

		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoName), (gchar *)a->GetName());

		// Protocol version
		gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaNodoEsclavoProtocolVersion), GetLinProtocolVersionStringID(db->GetLinProtocolVersion()));

		// Initial NAD
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoInitialNAD), GetStrPrintf("0x%02X", a->GetInitialNAD()));

		// Configured NAD
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoConfiguredNAD), GetStrPrintf("0x%02X", a->GetConfiguredNAD()));

		// Supplier ID
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoSupplierID), GetStrPrintf("0x%04X", a->GetSupplierID()));

		// Function ID
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoFunctionID), GetStrPrintf("0x%04X", a->GetFunctionID()));

		// Variant
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoVariant), GetStrPrintf("0x%02X", a->GetVariant()));

		// Response error signal
		gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(g_VentanaNodoEsclavoResponseErrorSignal));
		gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(g_VentanaNodoEsclavoResponseErrorSignal), "", "None");
		gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaNodoEsclavoResponseErrorSignal), "");
		// TODO: Load this list with all 1 bit signals published by this node. Leave this way for now

		// P2 min
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoP2_min), GetStrPrintf("%d", a->GetP2_min()));

		// ST min
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoST_min), GetStrPrintf("%d", a->GetST_min()));

		// N_As_timeout
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoN_As_timeout), GetStrPrintf("%d", a->GetN_As_timeout()));

		// N_Cr_timeout
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoN_Cr_timeout), GetStrPrintf("%d", a->GetN_Cr_timeout()));
	}

	// Connect text fields
	G_CONNECT_INSTXT(VentanaNodoEsclavoName, NAME_EXPR);
	G_CONNECT_INSTXT(VentanaNodoEsclavoInitialNAD, HEX2_EXPR);
	G_CONNECT_INSTXT(VentanaNodoEsclavoConfiguredNAD, HEX2_EXPR);
	G_CONNECT_INSTXT(VentanaNodoEsclavoSupplierID, HEX4_EXPR);
	G_CONNECT_INSTXT(VentanaNodoEsclavoFunctionID, HEX4_EXPR);
	G_CONNECT_INSTXT(VentanaNodoEsclavoVariant, HEX2_EXPR);
	G_CONNECT_INSTXT(VentanaNodoEsclavoP2_min, INT5_EXPR);
	G_CONNECT_INSTXT(VentanaNodoEsclavoST_min, INT5_EXPR);
	G_CONNECT_INSTXT(VentanaNodoEsclavoN_As_timeout, INT5_EXPR);
	G_CONNECT_INSTXT(VentanaNodoEsclavoN_Cr_timeout, INT5_EXPR);

	// Connect buttons
	G_CONNECT(VentanaNodoEsclavoConfigFrameNew, clicked);
	G_CONNECT(VentanaNodoEsclavoConfigFrameEdit, clicked);
	G_CONNECT(VentanaNodoEsclavoConfigFrameDelete, clicked);
	G_CONNECT(VentanaNodoEsclavoAccept, clicked);
	G_CONNECT(VentanaNodoEsclavoCancel, clicked);
}

VentanaNodoEsclavo::~VentanaNodoEsclavo()
{
	// Disconnect text fields
	G_DISCONNECT_FUNC(VentanaNodoEsclavoName, EditableInsertValidator);
	G_DISCONNECT_FUNC(VentanaNodoEsclavoInitialNAD, EditableInsertValidator);
	G_DISCONNECT_FUNC(VentanaNodoEsclavoConfiguredNAD, EditableInsertValidator);
	G_DISCONNECT_FUNC(VentanaNodoEsclavoSupplierID, EditableInsertValidator);
	G_DISCONNECT_FUNC(VentanaNodoEsclavoFunctionID, EditableInsertValidator);
	G_DISCONNECT_FUNC(VentanaNodoEsclavoVariant, EditableInsertValidator);
	G_DISCONNECT_FUNC(VentanaNodoEsclavoP2_min, EditableInsertValidator);
	G_DISCONNECT_FUNC(VentanaNodoEsclavoST_min, EditableInsertValidator);
	G_DISCONNECT_FUNC(VentanaNodoEsclavoN_As_timeout, EditableInsertValidator);
	G_DISCONNECT_FUNC(VentanaNodoEsclavoN_Cr_timeout, EditableInsertValidator);

	// Disconnect buttons
	G_DISCONNECT_DATA(VentanaNodoEsclavoConfigFrameNew, this);
	G_DISCONNECT_DATA(VentanaNodoEsclavoConfigFrameEdit, this);
	G_DISCONNECT_DATA(VentanaNodoEsclavoConfigFrameDelete, this);
	G_DISCONNECT_DATA(VentanaNodoEsclavoAccept, this);
	G_DISCONNECT_DATA(VentanaNodoEsclavoCancel, this);
}

ldfnodeattributes *VentanaNodoEsclavo::ShowModal(GObject *parent)
{
	ldfnodeattributes *res = NULL;

	// Put this window always on top of parent
	gtk_window_set_transient_for(GTK_WINDOW(handle), GTK_WINDOW(parent));

	// Show dialog
	if (gtk_dialog_run(GTK_DIALOG(handle)))
	{
		// Name
		res = new ldfnodeattributes((uint8_t *)gtk_entry_get_text(GTK_ENTRY(g_VentanaNodoEsclavoName)));

		// Initial NAD
		res->SetInitialNAD(MultiParseInt(gtk_entry_get_text(GTK_ENTRY(g_VentanaNodoEsclavoInitialNAD))));

		// Configured NAD
		res->SetConfiguredNAD(MultiParseInt(gtk_entry_get_text(GTK_ENTRY(g_VentanaNodoEsclavoConfiguredNAD))));

		// Supplier ID
		res->SetSupplierID(MultiParseInt(gtk_entry_get_text(GTK_ENTRY(g_VentanaNodoEsclavoSupplierID))));

		// Function ID
		res->SetFunctionID(MultiParseInt(gtk_entry_get_text(GTK_ENTRY(g_VentanaNodoEsclavoFunctionID))));

		// Variant
		res->SetVariant(MultiParseInt(gtk_entry_get_text(GTK_ENTRY(g_VentanaNodoEsclavoVariant))));

		// P2 min
		res->SetP2_min(MultiParseInt(gtk_entry_get_text(GTK_ENTRY(g_VentanaNodoEsclavoP2_min))));

		// ST min
		res->SetST_min(MultiParseInt(gtk_entry_get_text(GTK_ENTRY(g_VentanaNodoEsclavoST_min))));

		// N_As_timeout
		res->SetN_As_timeout(MultiParseInt(gtk_entry_get_text(GTK_ENTRY(g_VentanaNodoEsclavoN_As_timeout))));

		// N_Cr_timeout
		res->SetN_Cr_timeout(MultiParseInt(gtk_entry_get_text(GTK_ENTRY(g_VentanaNodoEsclavoN_Cr_timeout))));
	}
	gtk_widget_hide(GTK_WIDGET(handle));

	return res;
}

void VentanaNodoEsclavo::OnVentanaNodoEsclavoConfigFrameNew_clicked(GtkButton *button, gpointer user_data)
{
	VentanaNodoEsclavo *v = (VentanaNodoEsclavo *)user_data;

}

void VentanaNodoEsclavo::OnVentanaNodoEsclavoConfigFrameEdit_clicked(GtkButton *button, gpointer user_data)
{
	VentanaNodoEsclavo *v = (VentanaNodoEsclavo *)user_data;

}

void VentanaNodoEsclavo::OnVentanaNodoEsclavoConfigFrameDelete_clicked(GtkButton *button, gpointer user_data)
{
	VentanaNodoEsclavo *v = (VentanaNodoEsclavo *)user_data;

}

void VentanaNodoEsclavo::OnVentanaNodoEsclavoAccept_clicked(GtkButton *button, gpointer user_data)
{
	VentanaNodoEsclavo *v = (VentanaNodoEsclavo *)user_data;
	const char *new_slave_name = gtk_entry_get_text(GTK_ENTRY(v->g_VentanaNodoEsclavoName));
	ldfnodeattributes *attributes = v->db->GetSlaveNodeAttributes((uint8_t *)new_slave_name);

	// Validate slave name
	if (strlen(new_slave_name) == 0)
	{
		// Identifier invalid
		ShowErrorMessageBox(v->handle, "Incorrect slave name.");
		return;
	}

	// Validate node name
	if (v->slave_name == NULL)
	{
		if (attributes != NULL)
		{
			// Identifier in use
			ShowErrorMessageBox(v->handle, "Slave name '%s' is already in use.", new_slave_name);
			return;
		}
	}
	else
	{
		if (strcmp(new_slave_name, (const char *)v->slave_name) != 0 && attributes != NULL)
		{
			// Identifier in use
			ShowErrorMessageBox(v->handle, "Slave name changed to another one that '%s' is already in use.", new_slave_name);
			return;
		}
	}

	// Return true
	gtk_dialog_response(GTK_DIALOG(v->handle), true);
}

void VentanaNodoEsclavo::OnVentanaNodoEsclavoCancel_clicked(GtkButton *button, gpointer user_data)
{
	VentanaNodoEsclavo *v = (VentanaNodoEsclavo *)user_data;

	// Return false
	gtk_dialog_response(GTK_DIALOG(v->handle), false);
}


} /* namespace ui */
