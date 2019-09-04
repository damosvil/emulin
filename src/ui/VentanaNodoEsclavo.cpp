/*
 * VentanaNodoEsclavo.cpp
 *
 *  Created on: 7 ago. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <VentanaNodoEsclavo.h>
#include <VentanaConfigurableFrame.h>
#include "tools.h"

using namespace tools;

namespace ui {

VentanaNodoEsclavo::VentanaNodoEsclavo(GtkBuilder *builder, ldf *db, char *slave_name)
{
	// Store input info
	this->builder = builder;
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
	G_PIN(VentanaNodoEsclavoConfigFrameSelection);
	G_PIN(VentanaNodoEsclavoConfigFrameNew);
	G_PIN(VentanaNodoEsclavoConfigFrameEdit);
	G_PIN(VentanaNodoEsclavoConfigFrameDelete);
	G_PIN(VentanaNodoEsclavoAccept);
	G_PIN(VentanaNodoEsclavoCancel);

	// Initialize list views
	PrepareListConfigurableFrames();

	// Fill dialog fields with data
	ldfnodeattributes *a = (slave_name != NULL) ? db->GetSlaveNodeAttributesByName((uint8_t *)slave_name) : NULL;
	if (a != NULL)
	{
		// Name
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoName), (gchar *)a->GetName());

		// Protocol version
		gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaNodoEsclavoProtocolVersion), GetLinProtocolVersionStringID(a->GetProtocolVersion()));

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

		// Load response error signal list
		gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(g_VentanaNodoEsclavoResponseErrorSignal));
		for (uint32_t i = 0; i < db->GetSignalsCount(); i++)
		{
			// Skip signals not published by this node or size different to 1
			ldfsignal *s = db->GetSignalByIndex(i);
			if (s->GetBitSize() != 1 || !s->PublisherIs((uint8_t *)slave_name))
				continue;

			// Add signal to list
			gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(g_VentanaNodoEsclavoResponseErrorSignal), (char *)s->GetName(), (char *)s->GetName());
		}
		gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaNodoEsclavoResponseErrorSignal), (char *)a->GetResponseErrorSignalName());

		// P2 min
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoP2_min), GetStrPrintf("%d", a->GetP2_min()));

		// ST min
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoST_min), GetStrPrintf("%d", a->GetST_min()));

		// N_As_timeout
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoN_As_timeout), GetStrPrintf("%d", a->GetN_As_timeout()));

		// N_Cr_timeout
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoN_Cr_timeout), GetStrPrintf("%d", a->GetN_Cr_timeout()));

		// Load configurable frame list
		ReloadListConfigurableFrames();
	}
	else
	{
		// Name
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoName), "node");

		// Protocol version
		gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaNodoEsclavoProtocolVersion), GetLinProtocolVersionStringID(LIN_PROTOCOL_VERSION_2_1));

		// Initial NAD
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoInitialNAD), "0xFF");

		// Configured NAD
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoConfiguredNAD), "0xFF");

		// Supplier ID
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoSupplierID), "0x0000");

		// Function ID
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoFunctionID), "0x0000");

		// Variant(uint8_t *)
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoVariant), "0x00");

		// Response error signal
		gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(g_VentanaNodoEsclavoResponseErrorSignal));

		// P2 min
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoP2_min), "50");

		// ST min
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoST_min), "0");

		// N_As_timeout
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoN_As_timeout), "1000");

		// N_Cr_timeout
		gtk_entry_set_text(GTK_ENTRY(g_VentanaNodoEsclavoN_Cr_timeout), "1000");
	}

	// Connect text fields
	G_CONNECT_INSTXT(VentanaNodoEsclavoName, NAME_EXPR);
	G_CONNECT_INSTXT(VentanaNodoEsclavoInitialNAD, INT3_EXPR);
	G_CONNECT_INSTXT(VentanaNodoEsclavoConfiguredNAD, INT3_EXPR);
	G_CONNECT_INSTXT(VentanaNodoEsclavoSupplierID, INT5_EXPR);
	G_CONNECT_INSTXT(VentanaNodoEsclavoFunctionID, INT5_EXPR);
	G_CONNECT_INSTXT(VentanaNodoEsclavoVariant, INT3_EXPR);
	G_CONNECT_INSTXT(VentanaNodoEsclavoP2_min, INT5_EXPR);
	G_CONNECT_INSTXT(VentanaNodoEsclavoST_min, INT5_EXPR);
	G_CONNECT_INSTXT(VentanaNodoEsclavoN_As_timeout, INT5_EXPR);
	G_CONNECT_INSTXT(VentanaNodoEsclavoN_Cr_timeout, INT5_EXPR);

	// Connect lists
	G_CONNECT(VentanaNodoEsclavoConfigFrameList, row_activated);
	G_CONNECT(VentanaNodoEsclavoConfigFrameSelection, changed);

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

	// Disconnect lists
	G_DISCONNECT_DATA(VentanaNodoEsclavoConfigFrameList, this);
	G_DISCONNECT_DATA(VentanaNodoEsclavoConfigFrameSelection, this);

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

		// Protocol version
		res->SetProtocolVersion(GetProtocolVersionByStringID(gtk_combo_box_get_active_id(GTK_COMBO_BOX(g_VentanaNodoEsclavoProtocolVersion))));

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

		// Response error signal name
		const char *resn = gtk_combo_box_get_active_id(GTK_COMBO_BOX(g_VentanaNodoEsclavoResponseErrorSignal));
		if (resn != NULL)
			res->SetResponseErrorSignalName((uint8_t *)resn);

		// P2 min
		res->SetP2_min(MultiParseInt(gtk_entry_get_text(GTK_ENTRY(g_VentanaNodoEsclavoP2_min))));

		// ST min
		res->SetST_min(MultiParseInt(gtk_entry_get_text(GTK_ENTRY(g_VentanaNodoEsclavoST_min))));

		// N_As_timeout
		res->SetN_As_timeout(MultiParseInt(gtk_entry_get_text(GTK_ENTRY(g_VentanaNodoEsclavoN_As_timeout))));

		// N_Cr_timeout
		res->SetN_Cr_timeout(MultiParseInt(gtk_entry_get_text(GTK_ENTRY(g_VentanaNodoEsclavoN_Cr_timeout))));

		// Configurable frames
		char *frame_id;
		char *frame_name;
		GtkTreeIter iter;
		GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(g_VentanaNodoEsclavoConfigFrameList));
		if (gtk_tree_model_get_iter_first(model, &iter))
		{
			do
			{
				gtk_tree_model_get(model, &iter, 0, &frame_id, -1);
				gtk_tree_model_get(model, &iter, 1, &frame_name, -1);
				res->AddConfigurableFrame(new ldfconfigurableframe((uint8_t *)frame_name, MultiParseInt(frame_id)));
			}
			while (gtk_tree_model_iter_next(model, &iter));
		}
	}
	gtk_widget_hide(GTK_WIDGET(handle));

	return res;
}

void VentanaNodoEsclavo::PrepareListConfigurableFrames()
{
	const char *columns[] = { "ID", "Frame", NULL };

	// Prepare tree view
	TreeViewPrepare(g_VentanaNodoEsclavoConfigFrameList, columns);

	// Disable edit and delete buttons
	WidgetEnable(g_VentanaNodoEsclavoConfigFrameEdit, false);
	WidgetEnable(g_VentanaNodoEsclavoConfigFrameDelete, false);
}

void VentanaNodoEsclavo::ReloadListConfigurableFrames()
{
	GtkTreeIter it;
	GtkTreeView *v = GTK_TREE_VIEW(g_VentanaNodoEsclavoConfigFrameList);
	GtkListStore *s = GTK_LIST_STORE(gtk_tree_view_get_model(v));

	// Clear list store
	gtk_list_store_clear(s);

	// Skip if no slave
	ldfnodeattributes *a = db->GetSlaveNodeAttributesByName((uint8_t *)slave_name);
	if (a == NULL)
		return;

	// Add data to list store
	for (uint32_t i = 0; i < a->GetConfigurableFramesCount(); i++)
	{
		ldfconfigurableframe *f = a->GetConfigurableFrame(i);

		// Append a new list item
		gtk_list_store_append(s, &it);

		// ID
		gtk_list_store_set(s, &it, 0, GetStrPrintf("0x%02X", f->GetId()), -1);

		// name
		gtk_list_store_set(s, &it, 1, f->GetName(), -1);
	}
}

void VentanaNodoEsclavo::OnVentanaNodoEsclavoConfigFrameList_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
	VentanaNodoEsclavo *v = (VentanaNodoEsclavo *)user_data;

	gtk_widget_activate(GTK_WIDGET(v->g_VentanaNodoEsclavoConfigFrameEdit));
}

void VentanaNodoEsclavo::OnVentanaNodoEsclavoConfigFrameSelection_changed(GtkTreeSelection *widget, gpointer user_data)
{
	VentanaNodoEsclavo *v = (VentanaNodoEsclavo *)user_data;

	bool enable = gtk_tree_selection_count_selected_rows(widget) == 1;
	WidgetEnable(v->g_VentanaNodoEsclavoConfigFrameEdit, enable);
	WidgetEnable(v->g_VentanaNodoEsclavoConfigFrameDelete, enable);
}

void VentanaNodoEsclavo::OnVentanaNodoEsclavoConfigFrameNew_clicked(GtkButton *button, gpointer user_data)
{
	VentanaNodoEsclavo *v = (VentanaNodoEsclavo *)user_data;
	char *frame_names[1000];
	int frame_names_count = 0;
	VentanaConfigurableFrame::configurable_frame_raw_t configurable_frames[1000];
	int configurable_frames_count = 0;

	// Put in a list the frames this slave is publisher
	for (uint32_t i = 0; i < v->db->GetFramesCount(); i++)
	{
		ldfframe *f = v->db->GetFrameByIndex(i);

		// Skip other publishers' frames
		if (!f->PublisherIs((uint8_t*)v->slave_name))
			continue;

		frame_names[frame_names_count++] = (char *)f->GetName();
	}

	// Put in a list existing configurable frames for this slave
	GtkTreeIter iter;
	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(v->g_VentanaNodoEsclavoConfigFrameList));
	if (gtk_tree_model_get_iter_first(model, &iter))
	{
		do
		{
			gtk_tree_model_get(model, &iter, 0, &configurable_frames[configurable_frames_count].id, -1);
			gtk_tree_model_get(model, &iter, 1, &configurable_frames[configurable_frames_count].name, -1);
			configurable_frames_count++;
		}
		while (gtk_tree_model_iter_next(model, &iter));
	}

	// Remove already configured frames from frame_names_list
	for (int i = 0; i < frame_names_count; )
	{
		VentanaConfigurableFrame::configurable_frame_raw_t *c = NULL;

		// Skip frames not configured
		for (int j = 0; c == NULL && j < configurable_frames_count; j++)
		{
			c = Same(frame_names[i], configurable_frames[j].name) ? &configurable_frames[j] : NULL;
		}
		if (c == NULL)
		{
			i++;
			continue;
		}

		// Remove frame
		frame_names_count--;
		for (int j = i; j < frame_names_count; j++)
			frame_names[j] = frame_names[j + 1];
	}

	// Check if there is any frame to add to configurable frames list
	if (frame_names_count == 0)
	{
		ShowErrorMessageBox(v->handle, "There are no more suitable frames to choose from.");
		return;
	}

	VentanaConfigurableFrame w(v->builder, v->db, frame_names, frame_names_count, configurable_frames, configurable_frames_count);
	ldfconfigurableframe *f = w.ShowModal(v->handle);
	if (f == NULL)
		return;

	// Look for a place to insert the new configurable frame
	int ix = -1;
	for (int i = 0; (ix == -1) && i < configurable_frames_count; i++)
		if (MultiParseInt(configurable_frames[i].id) > f->GetId())
			ix = i;

	// Insert item in list store
	gtk_list_store_insert(GTK_LIST_STORE(model), &iter, ix);
	gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, (gchar *)GetStrPrintf("0x%02X", f->GetId()), -1);
	gtk_list_store_set(GTK_LIST_STORE(model), &iter, 1, (gchar *)f->GetName(), -1);

	// Delete configurable frame
	delete f;
}

void VentanaNodoEsclavo::OnVentanaNodoEsclavoConfigFrameEdit_clicked(GtkButton *button, gpointer user_data)
{
	VentanaNodoEsclavo *v = (VentanaNodoEsclavo *)user_data;
	char *frame_name;
	VentanaConfigurableFrame::configurable_frame_raw_t configurable_frames[1000];
	int configurable_frames_count = 0;

	GtkTreeIter iter;
	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(v->g_VentanaNodoEsclavoConfigFrameList));

	// Get selected frame_name
	gtk_tree_selection_get_selected(GTK_TREE_SELECTION(v->g_VentanaNodoEsclavoConfigFrameSelection), &model, &iter);
	gtk_tree_model_get(model, &iter, 1, &frame_name, -1);

	// Put in a list existing configurable frames for this slave
	if (gtk_tree_model_get_iter_first(model, &iter))
	{
		do
		{
			gtk_tree_model_get(model, &iter, 0, &configurable_frames[configurable_frames_count].id, -1);
			gtk_tree_model_get(model, &iter, 1, &configurable_frames[configurable_frames_count].name, -1);
			configurable_frames_count++;
		}
		while (gtk_tree_model_iter_next(model, &iter));
	}

	VentanaConfigurableFrame w(v->builder, v->db, &frame_name, 1, configurable_frames, configurable_frames_count);
	ldfconfigurableframe *f = w.ShowModal(v->handle);
	if (f == NULL)
		return;

	// Update configurable frame
	gtk_tree_selection_get_selected(GTK_TREE_SELECTION(v->g_VentanaNodoEsclavoConfigFrameSelection), &model, &iter);
	gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, (gchar *)GetStrPrintf("0x%02X", f->GetId()), -1);

	// Delete configurable frame
	delete f;
}

void VentanaNodoEsclavo::OnVentanaNodoEsclavoConfigFrameDelete_clicked(GtkButton *button, gpointer user_data)
{
	VentanaNodoEsclavo *v = (VentanaNodoEsclavo *)user_data;
	GtkTreeModel *model;
	GtkTreeIter iter;

	// Remove subscriber from list model
	gtk_tree_selection_get_selected(GTK_TREE_SELECTION(v->g_VentanaNodoEsclavoConfigFrameSelection), &model, &iter);
	gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
}

void VentanaNodoEsclavo::OnVentanaNodoEsclavoAccept_clicked(GtkButton *button, gpointer user_data)
{
	VentanaNodoEsclavo *v = (VentanaNodoEsclavo *)user_data;
	const char *new_slave_name = gtk_entry_get_text(GTK_ENTRY(v->g_VentanaNodoEsclavoName));
	ldfnodeattributes *attributes = v->db->GetSlaveNodeAttributesByName((uint8_t *)new_slave_name);

	// Validate slave name
	if (strlen(new_slave_name) == 0)
	{
		// Identifier invalid
		ShowErrorMessageBox(v->handle, "Incorrect slave name.");
		return;
	}
	else if (v->slave_name == NULL && attributes != NULL)
	{
		// Identifier in use
		ShowErrorMessageBox(v->handle, "Slave name '%s' is already in use.", new_slave_name);
		return;
	}
	else if (v->slave_name != NULL && !Same(new_slave_name, (char *)v->slave_name) && v->db->GetMasterNode()->NameIs((uint8_t *)new_slave_name))
	{
		// Identifier in use
		ShowErrorMessageBox(v->handle, "Slave name '%s' is already in use by master node.", new_slave_name);
		return;
	}
	else if (v->slave_name != NULL && !Same(new_slave_name, (char *)v->slave_name) && attributes != NULL)
	{
		// Identifier in use
		ShowErrorMessageBox(v->handle, "Slave name changed to another one that '%s' is already in use.", new_slave_name);
		return;
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
