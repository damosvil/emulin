/*
 * VentanaInicio.cpp
 *
 *  Created on: 22 jul. 2019
 *      Author: iso9660
 */

#include <stdlib.h>
#include "tools.h"
#include "ldfcommon.h"
#include "ManagerConfig.h"
#include "VentanaInicio.h"
#include "VentanaNodoEsclavo.h"
#include "VentanaSenal.h"
#include "VentanaFrame.h"
#include "VentanaScheduleTable.h"
#include <ui.h>


using namespace managers;
using namespace tools;


namespace ui {

VentanaInicio::VentanaInicio(GtkBuilder *builder)
{
	GtkFileFilter *p;

	// Initialize attributes
	this->db = NULL;
	this->builder = builder;
	handle = gtk_builder_get_object(builder, "VentanaInicio");

	// Pin widgets
	G_PIN(PanelConfiguracionDatabase);
	G_PIN(PanelDatabaseLinProtocolVersion);
	G_PIN(PanelDatabaseLinLanguageVersion);
	G_PIN(PanelDatabaseLinSpeed);
	G_PIN(PanelDatabaseMasterName);
	G_PIN(PanelDatabaseMasterTimebase);
	G_PIN(PanelDatabaseMasterJitter);
	G_PIN(PanelDatabaseSlavesList);
	G_PIN(PanelDatabaseSlavesSelection);
	G_PIN(PanelDatabaseSlavesNew);
	G_PIN(PanelDatabaseSlavesEdit);
	G_PIN(PanelDatabaseSlavesDelete);
	G_PIN(PanelDatabaseSignalsList);
	G_PIN(PanelDatabaseSignalsSelection);
	G_PIN(PanelDatabaseSignalsNew);
	G_PIN(PanelDatabaseSignalsEdit);
	G_PIN(PanelDatabaseSignalsDelete);
	G_PIN(PanelDatabaseFramesList);
	G_PIN(PanelDatabaseFramesSelection);
	G_PIN(PanelDatabaseFramesNew);
	G_PIN(PanelDatabaseFramesEdit);
	G_PIN(PanelDatabaseFramesDelete);
	G_PIN(PanelDatabaseScheduleTablesList);
	G_PIN(PanelDatabaseScheduleTablesSelection);
	G_PIN(PanelDatabaseScheduleTablesNew);
	G_PIN(PanelDatabaseScheduleTablesEdit);
	G_PIN(PanelDatabaseScheduleTablesDelete);

	// Connect Window signals
	g_signal_connect(handle, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// Set file filter pattern to file chooser button (setting it in glade didn't work for me)
	p = gtk_file_filter_new();
	gtk_file_filter_set_name(p, "LIN definition files");
	gtk_file_filter_add_pattern(p, "*.ldf");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(g_PanelConfiguracionDatabase), p);

	// Initialize list views
	PrepareListSlaves();
	PrepareListSignals();
	PrepareListFrames();
	PrepareListScheduleTables();

	// Connect widget signals
	G_CONNECT(PanelConfiguracionDatabase, file_set);
	G_CONNECT(PanelDatabaseLinProtocolVersion, changed);
	G_CONNECT(PanelDatabaseLinLanguageVersion, changed);
	G_CONNECT(PanelDatabaseLinSpeed, changed);
	G_CONNECT(PanelDatabaseMasterName, changed);
	G_CONNECT(PanelDatabaseMasterTimebase, changed);
	G_CONNECT(PanelDatabaseMasterJitter, changed);
	G_CONNECT_INSTXT(PanelDatabaseLinSpeed, INT5_EXPR);
	G_CONNECT_INSTXT(PanelDatabaseMasterName, NAME_EXPR);
	G_CONNECT_INSTXT(PanelDatabaseMasterTimebase, SFLOAT_EXPR);
	G_CONNECT_INSTXT(PanelDatabaseMasterJitter, SFLOAT_EXPR);
	G_CONNECT(PanelDatabaseSlavesNew, clicked);
	G_CONNECT(PanelDatabaseSlavesEdit, clicked);
	G_CONNECT(PanelDatabaseSlavesDelete, clicked);
	G_CONNECT(PanelDatabaseSlavesList, row_activated);
	G_CONNECT(PanelDatabaseSlavesSelection, changed);
	G_CONNECT(PanelDatabaseSignalsNew, clicked);
	G_CONNECT(PanelDatabaseSignalsEdit, clicked);
	G_CONNECT(PanelDatabaseSignalsDelete, clicked);
	G_CONNECT(PanelDatabaseSignalsList, row_activated);
	G_CONNECT(PanelDatabaseSignalsSelection, changed);
	G_CONNECT(PanelDatabaseFramesNew, clicked);
	G_CONNECT(PanelDatabaseFramesEdit, clicked);
	G_CONNECT(PanelDatabaseFramesDelete, clicked);
	G_CONNECT(PanelDatabaseFramesList, row_activated);
	G_CONNECT(PanelDatabaseFramesSelection, changed);
	G_CONNECT(PanelDatabaseScheduleTablesNew, clicked);
	G_CONNECT(PanelDatabaseScheduleTablesEdit, clicked);
	G_CONNECT(PanelDatabaseScheduleTablesDelete, clicked);
	G_CONNECT(PanelDatabaseScheduleTablesList, row_activated);
	G_CONNECT(PanelDatabaseScheduleTablesSelection, changed);

	// Load database
	ReloadDatabase();
}

VentanaInicio::~VentanaInicio()
{
	if (db != NULL) delete db;
}

void VentanaInicio::ReloadDatabase()
{
	const uint8_t *database_path = ManagerConfig::GetManager()->GetDatabasePath();

	// Check database path is valid
	if (database_path == NULL) return;

	// If database is loaded delete it and create a new one
	if (db != NULL) delete db;
	db = new ldf(database_path);

	// Pause all signal handlers
	G_PAUSE_DATA(PanelConfiguracionDatabase, this);
	G_PAUSE_DATA(PanelDatabaseLinProtocolVersion, this);
	G_PAUSE_DATA(PanelDatabaseLinLanguageVersion, this);
	G_PAUSE_DATA(PanelDatabaseLinSpeed, this);
	G_PAUSE_DATA(PanelDatabaseMasterName, this);
	G_PAUSE_DATA(PanelDatabaseMasterTimebase, this);
	G_PAUSE_DATA(PanelDatabaseMasterJitter, this);
	G_PAUSE_FUNC(PanelDatabaseLinSpeed, EditableInsertValidator);
	G_PAUSE_FUNC(PanelDatabaseMasterName, EditableInsertValidator);
	G_PAUSE_FUNC(PanelDatabaseMasterTimebase, EditableInsertValidator);
	G_PAUSE_FUNC(PanelDatabaseMasterJitter, EditableInsertValidator);

	// Set database path in file chooser
	gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(g_PanelConfiguracionDatabase), (char *)database_path);

	// Database LIN protocol version
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_PanelDatabaseLinProtocolVersion), GetLinProtocolVersionStringID(db->GetLinProtocolVersion()));

	// Database LIN language version
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_PanelDatabaseLinLanguageVersion), GetLinLanguageVersionStringID(db->GetLinLanguageVersion()));

	// Database LIN speed
	EntrySet(g_PanelDatabaseLinSpeed, "%d", db->GetLinSpeed());

	// Master's name
	EntrySet(g_PanelDatabaseMasterName, db->GetMasterNode()->GetName());

	// Master's timebase
	EntrySet(g_PanelDatabaseMasterTimebase, "%0.1f", (double)db->GetMasterNode()->GetTimebase() / 10.0f);

	// Master's jitter
	EntrySet(g_PanelDatabaseMasterJitter, "%0.1f", (double)db->GetMasterNode()->GetJitter() / 10.0f);

	// Sort database data
	db->SortData();

	// Reload lists
	ReloadListSlaves();
	ReloadListSignals();
	ReloadListFrames();
	ReloadListScheduleTables();

	// Play all signal handlers
	G_PLAY_DATA(PanelConfiguracionDatabase, this);
	G_PLAY_DATA(PanelDatabaseLinProtocolVersion, this);
	G_PLAY_DATA(PanelDatabaseLinLanguageVersion, this);
	G_PLAY_DATA(PanelDatabaseLinSpeed, this);
	G_PLAY_DATA(PanelDatabaseMasterName, this);
	G_PLAY_DATA(PanelDatabaseMasterTimebase, this);
	G_PLAY_DATA(PanelDatabaseMasterJitter, this);
	G_PLAY_FUNC(PanelDatabaseLinSpeed, EditableInsertValidator);
	G_PLAY_FUNC(PanelDatabaseMasterName, EditableInsertValidator);
	G_PLAY_FUNC(PanelDatabaseMasterTimebase, EditableInsertValidator);
	G_PLAY_FUNC(PanelDatabaseMasterJitter, EditableInsertValidator);
}

void VentanaInicio::PrepareListSlaves()
{
	const char *columns[] = { "Slave", "INAD", "CNAD", "ERR SIG", "CFG FRM", NULL };

	// Prepare tree view
	TreeViewPrepare(g_PanelDatabaseSlavesList, columns);

	// Disable edit and delete buttons
	WidgetEnable(g_PanelDatabaseSlavesEdit, FALSE);
	WidgetEnable(g_PanelDatabaseSlavesDelete, FALSE);
}

void VentanaInicio::ReloadListSlaves()
{
	uint32_t ix;
	GtkTreeIter it;
	GtkTreeView *v = GTK_TREE_VIEW(g_PanelDatabaseSlavesList);
	GtkListStore *s = GTK_LIST_STORE(gtk_tree_view_get_model(v));

	// Clear list store
	gtk_list_store_clear(s);

	// Add data to list store
	for (ix = 0; ix < db->GetSlaveNodesCount(); ix++)
	{
		ldfnodeattributes *a = db->GetSlaveNodeAttributesByName(db->GetSlaveNodeByIndex(ix)->GetName());

		// Slave name
		gtk_list_store_append(s, &it);
		gtk_list_store_set(s, &it, 0, a->GetName(), -1);

		// ldfnodeInitial node address
		gtk_list_store_set(s, &it, 1, GetStrPrintf("0x%02X", a->GetInitialNAD()), -1);

		// Configured node address

		gtk_list_store_set(s, &it, 2, GetStrPrintf("0x%02X", a->GetConfiguredNAD()), -1);

		// Response error signal name
		gtk_list_store_set(s, &it, 3, a->GetResponseErrorSignalName(), -1);

		// Configurable frames
		char str[100000];
		uint32_t jx;
		str[0] = 0;
		for (jx = 0; jx < a->GetConfigurableFramesCount(); jx++)
		{
			if (jx > 0) strcat(str, "\r\n");
			strcat(str, (char *)a->GetConfigurableFrame(jx)->GetName());
		}
		gtk_list_store_set(s, &it, 4, str, -1);
	}
}

void VentanaInicio::PrepareListSignals()
{
	const char *columns[] = { "Signal", "Size", "Ini.Val", "Publisher", "Subscribers", NULL };

	// Prepare tree view
	TreeViewPrepare(g_PanelDatabaseSignalsList, columns);

	// Disable edit and delete buttons
	WidgetEnable(g_PanelDatabaseSignalsEdit, FALSE);
	WidgetEnable(g_PanelDatabaseSignalsDelete, FALSE);
}

void VentanaInicio::ReloadListSignals()
{
	uint32_t ix;
	GtkTreeIter it;
	GtkTreeView *v = GTK_TREE_VIEW(g_PanelDatabaseSignalsList);
	GtkListStore *s = GTK_LIST_STORE(gtk_tree_view_get_model(v));

	// Clear list store
	gtk_list_store_clear(s);

	// Add data to list store
	for (ix = 0; ix < db->GetSignalsCount(); ix++)
	{
		ldfsignal *signal = db->GetSignalByIndex(ix);

		// Signal name
		gtk_list_store_append(s, &it);
		gtk_list_store_set(s, &it, 0, signal->GetName(), -1);

		// Bit size
		gtk_list_store_set(s, &it, 1, GetStrPrintf("%d", signal->GetBitSize()), -1);

		// Initial Value
		gtk_list_store_set(s, &it, 2, GetStrPrintf("0x%02X", signal->GetDefaultValue()), -1);

		// Publisher
		gtk_list_store_set(s, &it, 3, signal->GetPublisher(), -1);

		// Subscribers
		char str[100000];
		uint32_t jx;
		str[0] = 0;
		for (jx = 0; jx < signal->GetSubscribersCount(); jx++)
		{
			if (jx != 0) strcat(str, "\r\n");
			strcat(str, (char *)signal->GetSubscriber(jx));
		}
		gtk_list_store_set(s, &it, 4, str, -1);
	}
}

void VentanaInicio::PrepareListFrames()
{
	const char *columns[] = { "Frame", "ID", "Publisher", "Size", "Signals", NULL };

	// Prepare tree view
	TreeViewPrepare(g_PanelDatabaseFramesList, columns);

	// Disable edit and delete buttons
	WidgetEnable(g_PanelDatabaseFramesEdit, FALSE);
	WidgetEnable(g_PanelDatabaseFramesDelete, FALSE);
}

void VentanaInicio::ReloadListFrames()
{
	uint32_t ix;
	GtkTreeIter it;
	GtkTreeView *v = GTK_TREE_VIEW(g_PanelDatabaseFramesList);
	GtkListStore *s = GTK_LIST_STORE(gtk_tree_view_get_model(v));


	// Clear list store
	gtk_list_store_clear(s);

	// Add data to list store
	for (ix = 0; ix < db->GetFramesCount(); ix++)
	{
		ldfframe *frame = db->GetFrameByIndex(ix);

		// Frame name
		gtk_list_store_append(s, &it);
		gtk_list_store_set(s, &it, 0, frame->GetName(), -1);

		// ID
		gtk_list_store_set(s, &it, 1, GetStrPrintf("%d", frame->GetId()), -1);

		// Publisher
		gtk_list_store_set(s, &it, 2, frame->GetPublisher(), -1);

		// Size
		gtk_list_store_set(s, &it, 3, GetStrPrintf("%d", frame->GetSize()), -1);

		// Signals
		char str[100000];
		uint32_t jx;
		str[0] = 0;
		for (jx = 0; jx < frame->GetSignalsCount(); jx++)
		{
			if (jx != 0) strcat(str, "\r\n");
			strcat(str, GetStrPrintf("%02d: %s", frame->GetSignal(jx)->GetOffset(), frame->GetSignal(jx)->GetName()));
		}
		gtk_list_store_set(s, &it, 4, str, -1);
	}
}

void VentanaInicio::PrepareListScheduleTables()
{
	const char *columns[] = { "Name", "Cycle", "Frames", NULL };

	// Prepare tree view
	TreeViewPrepare(g_PanelDatabaseScheduleTablesList, columns);

	// Disable edit and delete buttons
	WidgetEnable(g_PanelDatabaseScheduleTablesEdit, FALSE);
	WidgetEnable(g_PanelDatabaseScheduleTablesDelete, FALSE);
}

void VentanaInicio::ReloadListScheduleTables()
{
	uint32_t ix;
	GtkTreeIter it;
	GtkTreeView *v = GTK_TREE_VIEW(g_PanelDatabaseScheduleTablesList);
	GtkListStore *s = GTK_LIST_STORE(gtk_tree_view_get_model(v));

	// Clear list store
	gtk_list_store_clear(s);

	// Add data to list store
	for (ix = 0; ix < db->GetScheduleTablesCount(); ix++)
	{
		ldfscheduletable *table = db->GetScheduleTableByIndex(ix);

		// Schedule table name
		gtk_list_store_append(s, &it);
		gtk_list_store_set(s, &it, 0, table->GetName(), -1);

		// Cycle
		uint32_t cycle = 0;
		for (int j = 0; j < table->GetCommandsCount(); j++)
			cycle += table->GetCommandByIndex(j)->GetTimeoutMs();
		gtk_list_store_set(s, &it, 1, GetStrPrintf("%d ms", cycle), -1);

		// Frames
		char str[100000];
		str[0] = 0;
		for (int j = 0; j < table->GetCommandsCount(); j++)
		{
			ldfschedulecommand *cc = table->GetCommandByIndex(j);

			if (j != 0) strcat(str, "\r\n");
			strcat(str, GetStrPrintf("(%d ms) %s", cc->GetTimeoutMs(), cc->GetStrCommand(db)));
		}
		gtk_list_store_set(s, &it, 2, str, -1);
	}
}

void VentanaInicio::OnPanelConfiguracionDatabase_file_set(GtkFileChooserButton *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	ManagerConfig::GetManager()->SetDatabasePath(Str(gtk_file_chooser_get_filename((GtkFileChooser *)widget)));
	ManagerConfig::GetManager()->Store();

	// Load new database in application
	v->ReloadDatabase();
}

void VentanaInicio::OnPanelDatabaseLinProtocolVersion_changed(GtkComboBox *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	v->db->SetLinProtocolVersion(GetProtocolVersionByStringID(gtk_combo_box_get_active_id(widget)));
}

void VentanaInicio::OnPanelDatabaseLinLanguageVersion_changed(GtkComboBox *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	v->db->SetLinLanguageVersion(GetLanguageVersionByStringID(gtk_combo_box_get_active_id(widget)));
}

void VentanaInicio::OnPanelDatabaseLinSpeed_changed(GtkCellEditable *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	v->db->SetLinSpeed(EntryGetInt(G_OBJECT(widget)));
}

void VentanaInicio::OnPanelDatabaseMasterName_changed(GtkCellEditable *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;
	const char *new_master_name = EntryGetStr(G_OBJECT(widget));

	// Check that the identifier is not being used by a slave
	if (v->db->GetSlaveNodeAttributesByName(Str(new_master_name)) != NULL)
	{
		ShowErrorMessageBox(v->handle, "Master name won't be updated because identifier is in use by a slave");

		// Undo changes
		g_signal_handlers_block_matched(widget, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, v);
		g_signal_handlers_block_matched(widget, G_SIGNAL_MATCH_FUNC, 0, 0, 0, (gpointer)EditableInsertValidator, 0);
		EntrySet(G_OBJECT(widget), v->db->GetMasterNode()->GetName());
		g_signal_handlers_unblock_matched(widget, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, v);
		g_signal_handlers_unblock_matched(widget, G_SIGNAL_MATCH_FUNC, 0, 0, 0, (gpointer)EditableInsertValidator, 0);
		return;
	}

	// Update node name and reload all lists
	v->db->UpdateMasterNodeName(v->db->GetMasterNode()->GetName(), Str(new_master_name));
	v->db->SortData();
	v->ReloadListSignals();
	v->ReloadListFrames();
}

void VentanaInicio::OnPanelDatabaseMasterTimebase_changed(GtkCellEditable *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	v->db->GetMasterNode()->SetTimebase((uint16_t)EntryGetFloat(G_OBJECT(widget)) * 10);
}

void VentanaInicio::OnPanelDatabaseMasterJitter_changed(GtkCellEditable *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	v->db->GetMasterNode()->SetJitter((uint16_t)EntryGetFloat(G_OBJECT(widget)) * 10);
}

void VentanaInicio::OnPanelDatabaseSlavesNew_clicked(GtkButton *button, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	// Check master node name is valid
	if (v->db->GetMasterNode()->GetName()[0] == 0)
	{
		ShowErrorMessageBox(v->handle, "Master node name shall be valid before adding slaves");
		return;
	}

	// Create slave window
	VentanaNodoEsclavo w(v->builder, v->db, NULL);
	ldfnodeattributes *na = w.ShowModal(v->handle);

	// Add slave node if any
	if (na == NULL) return;
	v->db->AddSlaveNode(na);

	// Reload slaves list
	v->db->SortData();
	v->ReloadListSlaves();
	v->ReloadListSignals();
}

void VentanaInicio::OnPanelDatabaseSlavesEdit_clicked(GtkButton *button, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *slave_name;

	// Get slave name
	gtk_tree_selection_get_selected(GTK_TREE_SELECTION(v->g_PanelDatabaseSlavesSelection), &model, &iter);
	gtk_tree_model_get(model, &iter, 0, &slave_name, -1);

	// Create slave window
	VentanaNodoEsclavo w(v->builder, v->db, slave_name);
	ldfnodeattributes *na = w.ShowModal(v->handle);

	// Update slave node
	if (na == NULL) return;
	v->db->UpdateSlaveNode(Str(slave_name), na);

	// Reload slaves list
	v->db->SortData();
	v->ReloadListSlaves();
	v->ReloadListSignals();
	v->ReloadListFrames();
	v->ReloadListScheduleTables();
}

void VentanaInicio::OnPanelDatabaseSlavesDelete_clicked(GtkButton *button, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *slave_name;

	// Get slave name
	gtk_tree_selection_get_selected(GTK_TREE_SELECTION(v->g_PanelDatabaseSlavesSelection), &model, &iter);
	gtk_tree_model_get(model, &iter, 0, &slave_name, -1);

	// Ask before deleting
	if (!ShowChooseMessageBox(v->handle, "Delete slave '%s'?", slave_name))
		return;

	// Delete slave node
	v->db->DeleteSlaveNode(Str(slave_name));

	// Reload slaves list
	v->db->SortData();
	v->ReloadListSlaves();
	v->ReloadListSignals();
	v->ReloadListFrames();
	v->ReloadListSlaves();
}

void VentanaInicio::OnPanelDatabaseSlavesList_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	gtk_widget_activate(GTK_WIDGET(v->g_PanelDatabaseSlavesEdit));
}

void VentanaInicio::OnPanelDatabaseSlavesSelection_changed(GtkTreeSelection *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	bool enable = gtk_tree_selection_count_selected_rows(widget) == 1;
	WidgetEnable(v->g_PanelDatabaseSlavesEdit, enable);
	WidgetEnable(v->g_PanelDatabaseSlavesDelete, enable);
}

void VentanaInicio::OnPanelDatabaseSignalsNew_clicked(GtkButton *button, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	// Check minimal data is valid
	if (v->db->GetMasterNode()->GetName()[0] == 0)
	{
		ShowErrorMessageBox(v->handle, "Master node name shall be valid before adding signals");
		return;
	}
	else if (v->db->GetSlaveNodesCount() == 0)
	{
		ShowErrorMessageBox(v->handle, "At least one slave node shall be defined before adding signals.");
		return;
	}

	// Read signal from the user
	VentanaSenal w(v->builder, v->db, NULL);
	ldfsignal *s = w.ShowModal(v->handle);
	if (s == NULL) return;

	// Store the new signal
	v->db->AddSignal(s);
	v->db->SortData();
	v->ReloadListSignals();
}

void VentanaInicio::OnPanelDatabaseSignalsEdit_clicked(GtkButton *button, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *signal_name;

	// Get signal name
	gtk_tree_selection_get_selected(GTK_TREE_SELECTION(v->g_PanelDatabaseSignalsSelection), &model, &iter);
	gtk_tree_model_get(model, &iter, 0, &signal_name, -1);

	// Read signal from the user
	VentanaSenal w(v->builder, v->db, signal_name);
	ldfsignal *s = w.ShowModal(v->handle);
	if (s == NULL) return;

	// Update signal
	v->db->UpdateSignal(Str(signal_name), s);
	v->db->SortData();
	v->ReloadListSlaves();
	v->ReloadListSignals();
	v->ReloadListFrames();
}

void VentanaInicio::OnPanelDatabaseSignalsDelete_clicked(GtkButton *button, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *signal_name;

	// Get signal name
	gtk_tree_selection_get_selected(GTK_TREE_SELECTION(v->g_PanelDatabaseSignalsSelection), &model, &iter);
	gtk_tree_model_get(model, &iter, 0, &signal_name, -1);

	// Ask before deleting
	if (!ShowChooseMessageBox(v->handle, "Delete signal '%s'?", signal_name))
		return;

	// Delete the signal
	v->db->DeleteSignal(Str(signal_name));
	v->db->SortData();
	v->ReloadListSlaves();
	v->ReloadListSignals();
	v->ReloadListFrames();
}

void VentanaInicio::OnPanelDatabaseSignalsList_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	gtk_widget_activate(GTK_WIDGET(v->g_PanelDatabaseSignalsEdit));
}

void VentanaInicio::OnPanelDatabaseSignalsSelection_changed(GtkTreeSelection *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	bool enable = gtk_tree_selection_count_selected_rows(widget) == 1;
	WidgetEnable(v->g_PanelDatabaseSignalsEdit, enable);
	WidgetEnable(v->g_PanelDatabaseSignalsDelete, enable);
}

void VentanaInicio::OnPanelDatabaseFramesNew_clicked(GtkButton *button, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	// Check minimal data is valid
	if (v->db->GetMasterNode()->GetName()[0] == 0)
	{
		ShowErrorMessageBox(v->handle, "Master node name shall be valid before adding frames");
		return;
	}
	else if (v->db->GetSlaveNodesCount() == 0)
	{
		ShowErrorMessageBox(v->handle, "At least one slave node shall be defined before adding frames.");
		return;
	}
	else if (v->db->GetSignalsCount() == 0)
	{
		ShowErrorMessageBox(v->handle, "At least one signal shall be defined before adding frames.");
		return;
	}

	// Read signal from the user
	VentanaFrame w(v->builder, v->db, NULL);
	ldfframe *s = w.ShowModal(v->handle);
	if (s == NULL) return;

	// Store the new signal
	v->db->AddFrame(s);
	v->db->SortData();
	v->ReloadListSignals();
	v->ReloadListFrames();
	// TODO: Reload schedule list
}

void VentanaInicio::OnPanelDatabaseFramesEdit_clicked(GtkButton *button, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *frame_name;

	// Get signal name
	gtk_tree_selection_get_selected(GTK_TREE_SELECTION(v->g_PanelDatabaseFramesSelection), &model, &iter);
	gtk_tree_model_get(model, &iter, 0, &frame_name, -1);

	// Read signal from the user
	VentanaFrame w(v->builder, v->db, frame_name);
	ldfframe *f = w.ShowModal(v->handle);
	if (f == NULL) return;

	// Update frame
	v->db->UpdateFrame(Str(frame_name), f);
	v->db->SortData();
	v->ReloadListSlaves();
	v->ReloadListSignals();
	v->ReloadListFrames();
	// TODO: Reload schedule list
}

void VentanaInicio::OnPanelDatabaseFramesDelete_clicked(GtkButton *button, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *frame_name;

	// Get frame name
	gtk_tree_selection_get_selected(GTK_TREE_SELECTION(v->g_PanelDatabaseFramesSelection), &model, &iter);
	gtk_tree_model_get(model, &iter, 0, &frame_name, -1);

	// Ask before deleting
	if (!ShowChooseMessageBox(v->handle, "Delete frame '%s'?", frame_name))
		return;

	// Delete the frame
	v->db->DeleteFrame(Str(frame_name));
	v->db->SortData();
	v->ReloadListSlaves();
	v->ReloadListSignals();
	v->ReloadListFrames();
	// TODO: Reload schedule list
}

void VentanaInicio::OnPanelDatabaseFramesList_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	gtk_widget_activate(GTK_WIDGET(v->g_PanelDatabaseFramesEdit));
}

void VentanaInicio::OnPanelDatabaseFramesSelection_changed(GtkTreeSelection *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	bool enable = gtk_tree_selection_count_selected_rows(widget) == 1;
	WidgetEnable(v->g_PanelDatabaseFramesEdit, enable);
	WidgetEnable(v->g_PanelDatabaseFramesDelete, enable);
}

void VentanaInicio::OnPanelDatabaseScheduleTablesNew_clicked(GtkButton *button, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	// Check minimal data is valid
	if (v->db->GetMasterNode()->GetName()[0] == 0)
	{
		ShowErrorMessageBox(v->handle, "Master node name shall be valid before adding schedule tables");
		return;
	}
	else if (v->db->GetSlaveNodesCount() == 0)
	{
		ShowErrorMessageBox(v->handle, "At least one slave node shall be defined before adding schedule tables.");
		return;
	}
	else if (v->db->GetSignalsCount() == 0)
	{
		ShowErrorMessageBox(v->handle, "At least one signal shall be defined before adding schedule tables.");
		return;
	}
	else if (v->db->GetFramesCount() == 0)
	{
		ShowErrorMessageBox(v->handle, "At least one frame shall be defined before adding schedule tables.");
		return;
	}

	// Ask user to create new schedule table
	VentanaScheduleTable w(v->builder, v->db, NULL);
	ldfscheduletable *t = w.ShowModal(v->handle);
	if (t == NULL)
		return;

	// Update schedule table
	v->db->AddScheduleTable(t);
	v->ReloadListScheduleTables();
}

void VentanaInicio::OnPanelDatabaseScheduleTablesEdit_clicked(GtkButton *button, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *schedule_table_name;

	// Get schedule table name
	gtk_tree_selection_get_selected(GTK_TREE_SELECTION(v->g_PanelDatabaseScheduleTablesSelection), &model, &iter);
	gtk_tree_model_get(model, &iter, 0, &schedule_table_name, -1);

	// Ask user to update schedule table data
	VentanaScheduleTable w(v->builder, v->db, schedule_table_name);
	ldfscheduletable *t = w.ShowModal(v->handle);
	if (t == NULL)
		return;

	// Update schedule table
	v->db->UpdateScheduleTable(Str(schedule_table_name), t);
	v->ReloadListScheduleTables();
}

void VentanaInicio::OnPanelDatabaseScheduleTablesDelete_clicked(GtkButton *button, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *schedule_table_name;

	// Get schedule table name
	gtk_tree_selection_get_selected(GTK_TREE_SELECTION(v->g_PanelDatabaseScheduleTablesSelection), &model, &iter);
	gtk_tree_model_get(model, &iter, 0, &schedule_table_name, -1);

	// Delete schedule table
	v->db->DeleteScheduleTable(Str(schedule_table_name));
	v->ReloadListScheduleTables();
}

void VentanaInicio::OnPanelDatabaseScheduleTablesList_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	gtk_widget_activate(GTK_WIDGET(v->g_PanelDatabaseScheduleTablesEdit));
}

void VentanaInicio::OnPanelDatabaseScheduleTablesSelection_changed(GtkTreeSelection *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	bool enable = gtk_tree_selection_count_selected_rows(widget) == 1;
	WidgetEnable(v->g_PanelDatabaseScheduleTablesEdit, enable);
	WidgetEnable(v->g_PanelDatabaseScheduleTablesDelete, enable);
}


} /* namespace lin */
