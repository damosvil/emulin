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
	const uint8_t *database_path = ManagerConfig::GetManager()->GetDatabasePath();

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

	// Set database path & load database
	if (database_path != NULL)
	{
		gtk_file_chooser_set_filename((GtkFileChooser *)o, (char *)database_path);
		db = new ldf(database_path);
	}

	// Connect file chooser signals
	g_signal_connect(o, "file-set", G_CALLBACK(OnDatabaseFileSelected), this);
}

VentanaInicio::~VentanaInicio()
{
}

void VentanaInicio::OnDatabaseFileSelected(GtkFileChooserButton *widget, gpointer user_data)
{
//	VentanaInicio *v = (VentanaInicio *)user_data;

	ManagerConfig::GetManager()->SetDatabasePath((uint8_t *)gtk_file_chooser_get_filename((GtkFileChooser *)widget));
	ManagerConfig::GetManager()->Store();
}

} /* namespace lin */
