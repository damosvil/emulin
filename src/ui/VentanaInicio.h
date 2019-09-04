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
	G_VAR(PanelDatabaseSlavesSelection);
	G_VAR(PanelDatabaseSlavesNew);
	G_VAR(PanelDatabaseSlavesEdit);
	G_VAR(PanelDatabaseSlavesDelete);
	G_VAR(PanelDatabaseSignalsList);
	G_VAR(PanelDatabaseSignalsSelection);
	G_VAR(PanelDatabaseSignalsNew);
	G_VAR(PanelDatabaseSignalsEdit);
	G_VAR(PanelDatabaseSignalsDelete);
	G_VAR(PanelDatabaseFramesList);
	G_VAR(PanelDatabaseFramesSelection);
	G_VAR(PanelDatabaseFramesNew);
	G_VAR(PanelDatabaseFramesEdit);
	G_VAR(PanelDatabaseFramesDelete);

	// Processes
	void ReloadDatabase();
	void PrepareListSlaves();
	void ReloadListSlaves();
	void PrepareListSignals();
	void ReloadListSignals();
	void PrepareListFrames();
	void ReloadListFrames();
	void PrepareListScheduleTables();
	void ReloadListScheduleTables();

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
	static void OnPanelDatabaseSlavesSelection_changed(GtkTreeSelection *widget, gpointer user_data);

	static void OnPanelDatabaseSignalsNew_clicked(GtkButton *button, gpointer user_data);
	static void OnPanelDatabaseSignalsEdit_clicked(GtkButton *button, gpointer user_data);
	static void OnPanelDatabaseSignalsDelete_clicked(GtkButton *button, gpointer user_data);
	static void OnPanelDatabaseSignalsList_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);
	static void OnPanelDatabaseSignalsSelection_changed(GtkTreeSelection *widget, gpointer user_data);

	static void OnPanelDatabaseFramesNew_clicked(GtkButton *button, gpointer user_data);
	static void OnPanelDatabaseFramesEdit_clicked(GtkButton *button, gpointer user_data);
	static void OnPanelDatabaseFramesDelete_clicked(GtkButton *button, gpointer user_data);
	static void OnPanelDatabaseFramesList_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);
	static void OnPanelDatabaseFramesSelection_changed(GtkTreeSelection *widget, gpointer user_data);

public:
	VentanaInicio(GtkBuilder *builder);
	virtual ~VentanaInicio();

};

} /* namespace lin */

#endif /* UI_VENTANAINICIO_H_ */
