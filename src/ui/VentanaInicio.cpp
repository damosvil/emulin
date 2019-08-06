/*
 * VentanaInicio.cpp
 *
 *  Created on: 22 jul. 2019
 *      Author: iso9660
 */

#include <stdlib.h>
#include "tools.h"
#include "ManagerConfig.h"
#include "VentanaInicio.h"


using namespace managers;


namespace ui {

static const char *lin_protocol_ids[] = { NULL, "1", "0" };
static const char *lin_language_ids[] = { NULL, "1", "0" };


VentanaInicio::VentanaInicio(GtkBuilder *builder)
{
	GtkFileFilter *p;

	// Get window handler
	this->builder = builder;
	this->handler = gtk_builder_get_object(builder, "VentanaInicio");

	// Pin widget pointers
	G_PIN(PanelConfiguracionDatabase);
	G_PIN(PanelDatabaseLinProtocolVersion);
	G_PIN(PanelDatabaseLinLanguageVersion);
	G_PIN(PanelDatabaseLinSpeed);
	G_PIN(PanelDatabaseMasterName);
	G_PIN(PanelDatabaseMasterTimebase);
	G_PIN(PanelDatabaseMasterJitter);

	// Initialize db
	db = NULL;

	// Connect Window signals
	g_signal_connect(this->handler, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// Set file filter pattern to file chooser button (setting it in glade didn't work for me)
	p = gtk_file_filter_new();
	gtk_file_filter_set_name(p, "LIN definition files");
	gtk_file_filter_add_pattern(p, "*.ldf");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(g_PanelConfiguracionDatabase), p);

	// Connect widget signals
	G_CONNECT(PanelConfiguracionDatabase, FileSet, "file-set");
	G_CONNECT(PanelDatabaseLinProtocolVersion, Changed, "changed");
	G_CONNECT(PanelDatabaseLinLanguageVersion, Changed, "changed");
	G_CONNECT(PanelDatabaseLinSpeed, Changed, "changed");
	G_CONNECT(PanelDatabaseMasterName, Changed, "changed");
	G_CONNECT(PanelDatabaseMasterTimebase, Changed, "changed");
	G_CONNECT(PanelDatabaseMasterJitter, Changed, "changed");
	g_signal_connect(g_PanelDatabaseLinSpeed, "insert-text", G_CALLBACK(EditableInsertValidator), INT_EXPR);
	g_signal_connect(g_PanelDatabaseMasterName, "insert-text", G_CALLBACK(EditableInsertValidator), NAME_EXPR);
	g_signal_connect(g_PanelDatabaseMasterTimebase, "insert-text", G_CALLBACK(EditableInsertValidator), SFLOAT_EXPR);
	g_signal_connect(g_PanelDatabaseMasterJitter, "insert-text", G_CALLBACK(EditableInsertValidator), SFLOAT_EXPR);

	// Load database
	ReloadDatabase();
}

VentanaInicio::~VentanaInicio()
{
	if (db != NULL) delete db;
}

void VentanaInicio::OnPanelConfiguracionDatabaseFileSet(GtkFileChooserButton *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	ManagerConfig::GetManager()->SetDatabasePath((uint8_t *)gtk_file_chooser_get_filename((GtkFileChooser *)widget));
	ManagerConfig::GetManager()->Store();

	// Load new database in application
	v->ReloadDatabase();
}

void VentanaInicio::OnPanelDatabaseLinProtocolVersionChanged(GtkComboBox *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	v->db->SetLinProtocolVersion((lin_protocol_version_e)GetStrIndexByID(
			lin_protocol_ids, ARR_SIZE(lin_protocol_ids),
			gtk_combo_box_get_active_id(widget)));
}

void VentanaInicio::OnPanelDatabaseLinLanguageVersionChanged(GtkComboBox *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	v->db->SetLinLanguageVersion((lin_language_version_e)GetStrIndexByID(
			lin_language_ids, ARR_SIZE(lin_language_ids),
			gtk_combo_box_get_active_id(widget)));
}

void VentanaInicio::OnPanelDatabaseLinSpeedChanged(GtkCellEditable *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	v->db->SetLinSpeed(atoi(gtk_entry_get_text(GTK_ENTRY(widget))));
}

void VentanaInicio::OnPanelDatabaseMasterNameChanged(GtkCellEditable *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	v->db->GetMasterNode()->SetName((uint8_t *)gtk_entry_get_text(GTK_ENTRY(widget)));
}

void VentanaInicio::OnPanelDatabaseMasterTimebaseChanged(GtkCellEditable *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	v->db->GetMasterNode()->SetTimebase(atof(gtk_entry_get_text(GTK_ENTRY(widget))) * 10);
}

void VentanaInicio::OnPanelDatabaseMasterJitterChanged(GtkCellEditable *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	v->db->GetMasterNode()->SetJitter((uint16_t)atof(gtk_entry_get_text(GTK_ENTRY(widget))) * 10);
}

void VentanaInicio::ReloadDatabase()
{
	const uint8_t *database_path = ManagerConfig::GetManager()->GetDatabasePath();
	char str[1000];

	// Check database path is valid
	if (database_path == NULL) return;

	// If database is loaded delete it and create a new one
	if (db != NULL) delete db;
	db = new ldf(database_path);

	// Pause all signal handlers
	G_PAUSE(PanelConfiguracionDatabase);
	G_PAUSE(PanelDatabaseLinProtocolVersion);
	G_PAUSE(PanelDatabaseLinLanguageVersion);
	G_PAUSE(PanelDatabaseLinSpeed);
	G_PAUSE(PanelDatabaseMasterName);
	G_PAUSE(PanelDatabaseMasterTimebase);
	G_PAUSE(PanelDatabaseMasterJitter);
	g_signal_handlers_block_matched(g_PanelDatabaseLinSpeed, G_SIGNAL_MATCH_FUNC, 0, 0, 0, (gpointer)EditableInsertValidator, 0);
	g_signal_handlers_block_matched(g_PanelDatabaseMasterName, G_SIGNAL_MATCH_FUNC, 0, 0, 0, (gpointer)EditableInsertValidator, 0);
	g_signal_handlers_block_matched(g_PanelDatabaseMasterTimebase, G_SIGNAL_MATCH_FUNC, 0, 0, 0, (gpointer)EditableInsertValidator, 0);
	g_signal_handlers_block_matched(g_PanelDatabaseMasterJitter, G_SIGNAL_MATCH_FUNC, 0, 0, 0, (gpointer)EditableInsertValidator, 0);

	// Set database path in file chooser
	gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(g_PanelConfiguracionDatabase), (char *)database_path);

	// Database LIN protocol version
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_PanelDatabaseLinProtocolVersion), lin_protocol_ids[db->GetLinProtocolVersion()]);

	// Database LIN language version
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_PanelDatabaseLinLanguageVersion), lin_language_ids[db->GetLinLanguageVersion()]);

	// Database LIN speed
	sprintf(str, "%d", db->GetLinSpeed());
	gtk_entry_set_text(GTK_ENTRY(g_PanelDatabaseLinSpeed), str);

	// Master's name
	gtk_entry_set_text(GTK_ENTRY(g_PanelDatabaseMasterName), (char *)db->GetMasterNode()->GetName());

	// Master's timebase
	sprintf(str, "%0.1f", (double)db->GetMasterNode()->GetTimebase() / 10.0f);
	gtk_entry_set_text(GTK_ENTRY(g_PanelDatabaseMasterTimebase), str);

	// Master's jitter
	sprintf(str, "%0.1f", (double)db->GetMasterNode()->GetJitter() / 10.0f);
	gtk_entry_set_text(GTK_ENTRY(g_PanelDatabaseMasterJitter), str);

	// Play all signal handlers
	G_PLAY(PanelConfiguracionDatabase);
	G_PLAY(PanelDatabaseLinProtocolVersion);
	G_PLAY(PanelDatabaseLinLanguageVersion);
	G_PLAY(PanelDatabaseLinSpeed);
	G_PLAY(PanelDatabaseMasterName);
	G_PLAY(PanelDatabaseMasterTimebase);
	G_PLAY(PanelDatabaseMasterJitter);
	g_signal_handlers_unblock_matched(g_PanelDatabaseLinSpeed, G_SIGNAL_MATCH_FUNC, 0, 0, 0, (gpointer)EditableInsertValidator, 0);
	g_signal_handlers_unblock_matched(g_PanelDatabaseMasterName, G_SIGNAL_MATCH_FUNC, 0, 0, 0, (gpointer)EditableInsertValidator, 0);
	g_signal_handlers_unblock_matched(g_PanelDatabaseMasterTimebase, G_SIGNAL_MATCH_FUNC, 0, 0, 0, (gpointer)EditableInsertValidator, 0);
	g_signal_handlers_unblock_matched(g_PanelDatabaseMasterJitter, G_SIGNAL_MATCH_FUNC, 0, 0, 0, (gpointer)EditableInsertValidator, 0);
}

} /* namespace lin */
