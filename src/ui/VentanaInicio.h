/*
 * VentanaInicio.h
 *
 *  Created on: 22 jul. 2019
 *      Author: iso9660
 */

#ifndef UI_VENTANAINICIO_H_
#define UI_VENTANAINICIO_H_

#include "tools.h"
#include <gtk/gtk.h>
#include <ldf.h>

using namespace lin;

namespace ui {

class VentanaInicio {

private:
	ldf *db;
	GtkBuilder *builder;
	GObject *handler;

	// Widgets
	G_VAR(PanelConfiguracionDatabase);
	G_VAR(PanelDatabaseLinProtocolVersion);
	G_VAR(PanelDatabaseLinLanguageVersion);
	G_VAR(PanelDatabaseLinSpeed);
	G_VAR(PanelDatabaseMasterName);
	G_VAR(PanelDatabaseMasterTimebase);
	G_VAR(PanelDatabaseMasterJitter);

	// Signal events
	static void OnPanelConfiguracionDatabaseFileSet(GtkFileChooserButton *widget, gpointer user_data);
	static void OnPanelDatabaseLinProtocolVersionChanged(GtkComboBox *widget, gpointer user_data);
	static void OnPanelDatabaseLinLanguageVersionChanged(GtkComboBox *widget, gpointer user_data);
	static void OnPanelDatabaseLinSpeedChanged(GtkCellEditable *widget, gpointer user_data);
	static void OnPanelDatabaseMasterNameChanged(GtkCellEditable *widget, gpointer user_data);
	static void OnPanelDatabaseMasterTimebaseChanged(GtkCellEditable *widget, gpointer user_data);
	static void OnPanelDatabaseMasterJitterChanged(GtkCellEditable *widget, gpointer user_data);

	void PrepareSlavesList();
	void ReloadDatabase();

public:
	VentanaInicio(GtkBuilder *builder);
	virtual ~VentanaInicio();

};

} /* namespace lin */

#endif /* UI_VENTANAINICIO_H_ */
