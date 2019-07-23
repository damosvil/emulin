/*
 * VentanaInicio.h
 *
 *  Created on: 22 jul. 2019
 *      Author: iso9660
 */

#ifndef UI_VENTANAINICIO_H_
#define UI_VENTANAINICIO_H_

#include <gtk/gtk.h>
#include <ldf.h>

using namespace lin;

namespace ui {

class VentanaInicio {

private:
	ldf *db;
	GtkBuilder *builder;
	GObject *handler;

	static void OnDatabaseFileSelected(GtkFileChooserButton *widget, gpointer user_data);

	void ReloadDatabase();

public:
	VentanaInicio(GtkBuilder *builder);
	virtual ~VentanaInicio();

};

} /* namespace lin */

#endif /* UI_VENTANAINICIO_H_ */
