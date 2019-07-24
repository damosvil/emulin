/*
 * VentanaInicio.cpp
 *
 *  Created on: 22 jul. 2019
 *      Author: iso9660
 */

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

	// Connect input filters on events
	g_signal_connect(g_PanelDatabaseLinSpeed, "key-press-event", G_CALLBACK(KeyFilterNumbers), NULL);

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

	v->db->SetLinSpeed(atoi(gtk_entry_get_text(GTK_ENTRY(widget)))); // @suppress("Invalid arguments")
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

	// Stop all signal handlers
	G_STOP(PanelConfiguracionDatabase);
	G_STOP(PanelDatabaseLinProtocolVersion);
	G_STOP(PanelDatabaseLinLanguageVersion);
	G_STOP(PanelDatabaseLinSpeed);

	// Set database path in file chooser
	gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(g_PanelConfiguracionDatabase), (char *)database_path);

	// Database LIN protocol version
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_PanelDatabaseLinProtocolVersion), lin_protocol_ids[db->GetLinProtocolVersion()]);

	// Database LIN language version
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_PanelDatabaseLinLanguageVersion), lin_language_ids[db->GetLinLanguageVersion()]);

	// Database LIN speed
	sprintf(str, "%d", db->GetLinSpeed());
	gtk_entry_set_text(GTK_ENTRY(g_PanelDatabaseLinSpeed), str);

	// Run all signal handlers
	G_RUN(PanelConfiguracionDatabase);
	G_RUN(PanelDatabaseLinProtocolVersion);
	G_RUN(PanelDatabaseLinLanguageVersion);
	G_RUN(PanelDatabaseLinSpeed);
}

} /* namespace lin */
