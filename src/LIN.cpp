/*
 ============================================================================
 Name        : LIN.cpp
 Author      : ISO9660
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C++,
 ============================================================================
 */

#include <iostream>
#include <stdexcept>
#include <gtk/gtk.h>
#include "ldf.h"

using namespace std;

#define FILENAME				"config/database.ldf"

int main(int argc, char *argv[]) {
	GtkBuilder *builder;
	GError *error = NULL;
	lin::ldf *db = NULL;

	// Initialize gtk
	gtk_init(&argc, &argv);
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "src/ui/Emulin.glade", &error);

	// Instantiate database
	db = new lin::ldf((uint8_t *)FILENAME);

	// Resource erasing
	delete db;

	return 0;
}
