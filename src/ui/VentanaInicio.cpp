/*
 * VentanaInicio.cpp
 *
 *  Created on: 22 jul. 2019
 *      Author: iso9660
 */

#include "VentanaInicio.h"


namespace ui {

VentanaInicio::VentanaInicio(GtkBuilder *builder)
{
	// Get window handler
	this->builder = builder;
	this->handler = gtk_builder_get_object(builder, "VentanaInicio");

	// Connect signals
	g_signal_connect(this->handler, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

VentanaInicio::~VentanaInicio() {
	// TODO Auto-generated destructor stub
}

} /* namespace lin */
