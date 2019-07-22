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

	// Get window handler
	this->builder = builder;
	this->handler = gtk_builder_get_object(builder, "VentanaInicio");

	// Connect signals
	g_signal_connect(this->handler, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// Set database path
	o = gtk_builder_get_object(builder, "PanelConfiguracionDatabase");
	gtk_file_chooser_set_filename((GtkFileChooser *)o, "config/database.ldf");

	// Set file filter pattern to file chooser button (setting it in glade didn't work for me)
	p = gtk_file_filter_new();
	gtk_file_filter_set_name(p, "LIN definition files");
	gtk_file_filter_add_pattern(p, "*.ldf");
	gtk_file_chooser_add_filter((GtkFileChooser *)o, p);

	// Load database
	db = new ldf(ManagerConfig::GetManager()->GetDatabasePath());
}

VentanaInicio::~VentanaInicio() {
	// TODO Auto-generated destructor stub
}

} /* namespace lin */
