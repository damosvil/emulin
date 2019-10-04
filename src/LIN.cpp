/*
 ============================================================================
 Name        : LIN.cpp
 Author      : ISO9660
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C++,
 ============================================================================
 */

#include <locale.h>
#include <gtk/gtk.h>
#include <VentanaInicio.h>
#include <tools.h>


using namespace std;
using namespace ui;
using namespace tools;


int main(int argc, char *argv[])
{
	GtkBuilder *builder;
	GError *error = NULL;

	// Initialize GTK
	gtk_init(&argc, &argv);

	// Change locale because GTK changes it to the system one. Needed for floating point parsing.
	if (setlocale(LC_ALL, "POSIX") != NULL)
	{
		// Start the user interface
		builder = gtk_builder_new();
		gtk_builder_add_from_file(builder, "src/ui/Emulin.glade", &error);

		// Instantiate VentanaInicio
		VentanaInicio v(builder);

		// Run GTK's main loop
		gtk_main();
	}
	else
	{
		ShowErrorMessageBox(NULL, "Emulin cannot start because for any reason POSIX locale is not installed in this system.");
	}

	return 0;
}
