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
	GObject *handle;

	// Widgets
	G_VAR(PanelConfiguracionDatabase);
	G_VAR(PanelDatabaseLinProtocolVersion);
	G_VAR(PanelDatabaseLinLanguageVersion);
	G_VAR(PanelDatabaseLinSpeed);
	G_VAR(PanelDatabaseMasterName);
	G_VAR(PanelDatabaseMasterTimebase);
	G_VAR(PanelDatabaseMasterJitter);
	G_VAR(PanelDatabaseSlavesList);
	G_VAR(PanelDatabaseSlavesNew);
	G_VAR(PanelDatabaseSlavesEdit);
	G_VAR(PanelDatabaseSlavesDelete);
	GObject *g_PanelDatabaseSlavesListSelection;

	// Signal events
	static void OnPanelConfiguracionDatabase_file_set(GtkFileChooserButton *widget, gpointer user_data);
	static void OnPanelDatabaseLinProtocolVersion_changed(GtkComboBox *widget, gpointer user_data);
	static void OnPanelDatabaseLinLanguageVersion_changed(GtkComboBox *widget, gpointer user_data);
	static void OnPanelDatabaseLinSpeed_changed(GtkCellEditable *widget, gpointer user_data);
	static void OnPanelDatabaseMasterName_changed(GtkCellEditable *widget, gpointer user_data);
	static void OnPanelDatabaseMasterTimebase_changed(GtkCellEditable *widget, gpointer user_data);
	static void OnPanelDatabaseMasterJitter_changed(GtkCellEditable *widget, gpointer user_data);
	static void OnPanelDatabaseSlavesNew_clicked(GtkButton *button, gpointer user_data);
	static void OnPanelDatabaseSlavesEdit_clicked(GtkButton *button, gpointer user_data);
	static void OnPanelDatabaseSlavesDelete_clicked(GtkButton *button, gpointer user_data);
	static void OnPanelDatabaseSlavesList_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);
	static void OnPanelDatabaseSlavesListSelection_changed(GtkTreeSelection *widget, gpointer user_data);

	void PrepareSlavesList();
	void ReloadDatabase();
	void ReloadSlavesList(ldf *db);

public:
	VentanaInicio(GtkBuilder *builder);
	virtual ~VentanaInicio();

};

} /* namespace lin */

#endif /* UI_VENTANAINICIO_H_ */
