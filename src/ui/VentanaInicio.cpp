/*
 * VentanaInicio.cpp
 *
 *  Created on: 22 jul. 2019
 *      Author: iso9660
 */

#include "VentanaInicio.h"
#include "ManagerConfig.h"

using namespace managers;


namespace ui {

VentanaInicio::VentanaInicio(GtkBuilder *builder)
{
	GObject *o;
	GtkFileFilter *p;

	// Inicialize db
	db = NULL;

	// Get window handler
	this->builder = builder;
	this->handler = gtk_builder_get_object(builder, "VentanaInicio");

	// Connect Window signals
	g_signal_connect(this->handler, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// Set file filter pattern to file chooser button (setting it in glade didn't work for me)
	o = gtk_builder_get_object(builder, "PanelConfiguracionDatabase");
	p = gtk_file_filter_new();
	gtk_file_filter_set_name(p, "LIN definition files");
	gtk_file_filter_add_pattern(p, "*.ldf");
	gtk_file_chooser_add_filter((GtkFileChooser *)o, p);

	ReloadDatabase();

	// Connect file chooser signals
	g_signal_connect(o, "file-set", G_CALLBACK(OnDatabaseFileSelected), this);
}

VentanaInicio::~VentanaInicio()
{
	if (db != NULL) delete db;
}

void VentanaInicio::OnDatabaseFileSelected(GtkFileChooserButton *widget, gpointer user_data)
{
	VentanaInicio *v = (VentanaInicio *)user_data;

	ManagerConfig::GetManager()->SetDatabasePath((uint8_t *)gtk_file_chooser_get_filename((GtkFileChooser *)widget));
	ManagerConfig::GetManager()->Store();

	// Load new database in application
	v->ReloadDatabase();
}

void VentanaInicio::ReloadDatabase()
{
	const char *lin_protocol_ids[] = { "", "1", "0" };
	const char *lin_language_ids[] = { "", "1", "0" };
	const uint16_t lin_speeds[] = { 9600, 19200, 0 };
	const char *lin_speed_ids[] = { "1", "0", "0" };
	const uint8_t *database_path = ManagerConfig::GetManager()->GetDatabasePath();
	GObject *o = gtk_builder_get_object(builder, "PanelConfiguracionDatabase");
	uint32_t ix;

	// Release previous database
	if (db != NULL) delete db;

	// Set database path & load database
	if (database_path != NULL)
	{
		gtk_file_chooser_set_filename((GtkFileChooser *)o, (char *)database_path);
		db = new ldf(database_path);
	}

	// Database LIN protocol version
	o = gtk_builder_get_object(builder, "PanelDatabaseLinProtocolVersion");
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(o), lin_protocol_ids[db->GetLinProtocolVersion()]);

	// Database LIN language version
	o = gtk_builder_get_object(builder, "PanelDatabaseLinLanguageVersion");
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(o), lin_language_ids[db->GetLinLanguageVersion()]);

	// Database LIN speed
	o = gtk_builder_get_object(builder, "PanelDatabaseLinSpeed");
	while ((lin_speeds[ix] != 0) && (lin_speeds[ix] != db->GetLinSpeed())) ix++;	// << Select combo box speed
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(o), lin_speed_ids[ix]);
}

} /* namespace lin */
