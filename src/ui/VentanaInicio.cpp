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
	G_PIN(PanelDatabaseSlavesNew);
	G_PIN(PanelDatabaseSlavesEdit);
	G_PIN(PanelDatabaseSlavesDelete);
	g_PanelDatabaseSlavesListSelection = (GObject *)gtk_tree_view_get_selection(GTK_TREE_VIEW(g_PanelDatabaseSlavesList));

	// Connect Window signals
	g_signal_connect(handle, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// Set file filter pattern to file chooser button (setting it in glade didn't work for me)
	p = gtk_file_filter_new();
	gtk_file_filter_set_name(p, "LIN definition files");
	gtk_file_filter_add_pattern(p, "*.ldf");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(g_PanelConfiguracionDatabase), p);

	// Initialize slaves list view
	PrepareSlavesList();

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
	G_CONNECT(PanelDatabaseSlavesListSelection, changed);

	// Load database
	ReloadDatabase();
}

VentanaInicio::~VentanaInicio()
{
	if (db != NULL) delete db;
}

void VentanaInicio::OnPanelConfiguracionDatabase_file_set(GtkFileChooserButton *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	ManagerConfig::GetManager()->SetDatabasePath((uint8_t *)gtk_file_chooser_get_filename((GtkFileChooser *)widget));
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

	v->db->SetLinSpeed(atoi(gtk_entry_get_text(GTK_ENTRY(widget))));
}

void VentanaInicio::OnPanelDatabaseMasterName_changed(GtkCellEditable *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	v->db->GetMasterNode()->SetName((uint8_t *)gtk_entry_get_text(GTK_ENTRY(widget)));
}

void VentanaInicio::OnPanelDatabaseMasterTimebase_changed(GtkCellEditable *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	v->db->GetMasterNode()->SetTimebase(atof(gtk_entry_get_text(GTK_ENTRY(widget))) * 10);
}

void VentanaInicio::OnPanelDatabaseMasterJitter_changed(GtkCellEditable *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	v->db->GetMasterNode()->SetJitter((uint16_t)atof(gtk_entry_get_text(GTK_ENTRY(widget))) * 10);
}

void VentanaInicio::OnPanelDatabaseSlavesNew_clicked(GtkButton *button, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	// Create slave window
	VentanaNodoEsclavo w(v->builder, v->db, NULL);
	ldfnodeattributes *na = w.ShowModal(v->handle);

	// Add slave node if any
	if (na == NULL) return;
	v->db->AddSlaveNode(na);

	// Reload slaves list
	v->ReloadSlavesList(v->db);
}

void VentanaInicio::OnPanelDatabaseSlavesEdit_clicked(GtkButton *button, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *slave_name;

	// Get slave name
	gtk_tree_selection_get_selected(GTK_TREE_SELECTION(v->g_PanelDatabaseSlavesListSelection), &model, &iter);
	gtk_tree_model_get(model, &iter, 0, &slave_name, -1);

	// Create slave window
	VentanaNodoEsclavo w(v->builder, v->db, (uint8_t *)slave_name);
	ldfnodeattributes *na = w.ShowModal(v->handle);

	// Update slave node
	if (na == NULL) return;
	v->db->UpdateSlaveNode((uint8_t *)slave_name, na);

	// Reload slaves list
	v->ReloadSlavesList(v->db);
}

void VentanaInicio::OnPanelDatabaseSlavesDelete_clicked(GtkButton *button, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *slave_name;

	// Get slave name
	gtk_tree_selection_get_selected(GTK_TREE_SELECTION(v->g_PanelDatabaseSlavesListSelection), &model, &iter);
	gtk_tree_model_get(model, &iter, 0, &slave_name, -1);

	// Delete slave node
	v->db->DeleteSlaveNode((uint8_t *)slave_name);

	// Reload slaves list
	v->ReloadSlavesList(v->db);
}

void VentanaInicio::OnPanelDatabaseSlavesList_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	gtk_widget_activate(GTK_WIDGET(v->g_PanelDatabaseSlavesEdit));
}

void VentanaInicio::OnPanelDatabaseSlavesListSelection_changed(GtkTreeSelection *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	bool enable = gtk_tree_selection_count_selected_rows(widget) == 1;
	gtk_widget_set_sensitive(GTK_WIDGET(v->g_PanelDatabaseSlavesEdit), enable);
	gtk_widget_set_sensitive(GTK_WIDGET(v->g_PanelDatabaseSlavesDelete), enable);
}

void VentanaInicio::PrepareSlavesList()
{
	GtkListStore *s = gtk_list_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	GtkTreeView *v = GTK_TREE_VIEW(gtk_builder_get_object(builder, "PanelDatabaseSlavesList"));

	// Add columns
	TreeViewAddColumn(v, "Slave", 0);
	TreeViewAddColumn(v, "INAD", 1);
	TreeViewAddColumn(v, "CNAD", 2);
	TreeViewAddColumn(v, "ERR SIG", 3);
	TreeViewAddColumn(v, "CFG FRM", 4);

	// Set model and unmanage reference from this code
	gtk_tree_view_set_model(v, GTK_TREE_MODEL(s));
	g_object_unref(s);

	// Disable edit and delete buttons
	gtk_widget_set_sensitive(GTK_WIDGET(g_PanelDatabaseSlavesEdit), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(g_PanelDatabaseSlavesDelete), FALSE);
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
	gtk_entry_set_text(GTK_ENTRY(g_PanelDatabaseLinSpeed), GetStrPrintf("%d", db->GetLinSpeed()));

	// Master's name
	gtk_entry_set_text(GTK_ENTRY(g_PanelDatabaseMasterName), (char *)db->GetMasterNode()->GetName());

	// Master's timebase
	gtk_entry_set_text(GTK_ENTRY(g_PanelDatabaseMasterTimebase), GetStrPrintf("%0.1f", (double)db->GetMasterNode()->GetTimebase() / 10.0f));

	// Master's jitter
	gtk_entry_set_text(GTK_ENTRY(g_PanelDatabaseMasterJitter), GetStrPrintf("%0.1f", (double)db->GetMasterNode()->GetJitter() / 10.0f));

	// Slaves list
	ReloadSlavesList(db);

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

void VentanaInicio::ReloadSlavesList(ldf *db)
{
	uint32_t ix, jx;
	GtkTreeIter it;
	GtkTreeView *v = GTK_TREE_VIEW(gtk_builder_get_object(builder, "PanelDatabaseSlavesList"));
	GtkListStore *s = GTK_LIST_STORE(gtk_tree_view_get_model(v));

	// Clear list store
	gtk_list_store_clear(s);

	// Add data to list store
	for (ix = 0; ix < db->GetSlaveNodesCount(); ix++)
	{
		ldfnodeattributes *a = db->GetSlaveNode(db->GetSlaveNodes()[ix]->GetName());

		// Slave name
		gtk_list_store_append(s, &it);
		gtk_list_store_set(s, &it, 0, a->GetName(), -1);

		// ldfnodeInitial node address
		gtk_list_store_set(s, &it, 1, GetStrPrintf("0x%X", a->GetInitialNAD()), -1);

		// Configured node address
		gtk_list_store_set(s, &it, 2, GetStrPrintf("0x%X", a->GetConfiguredNAD()), -1);

		// Response error signal name
		gtk_list_store_set(s, &it, 3, a->GetResponseErrorSignalName(), -1);

		// Configurable frames
		char str[100000];
		str[0] = 0;
		for (jx = 0; jx < a->GetConfigurableFramesCount(); jx++)
		{
			if (jx > 0) strcat(str, ", ");
			strcat(str, (char *)a->GetConfigurableFrames()[jx]->GetName());
		}
		gtk_list_store_set(s, &it, 4, str, -1);
	}
}

} /* namespace lin */
