/*
 * VentanaScheduleTable.h
 *
 *  Created on: 6 sept. 2019
 *      Author: iso9660
 */

#ifndef UI_VENTANASCHEDULETABLE_H_
#define UI_VENTANASCHEDULETABLE_H_

#include "tools.h"
#include "ldf.h"

using namespace tools;
using namespace lin;


namespace ui
{

class VentanaScheduleTable
{

private:
	GtkBuilder *builder;
	ldf *db;
	const char *schedule_table_name;
	GObject *handle;

	// Widgets
	G_VAR(VentanaScheduleTableName);
	G_VAR(VentanaScheduleTableList);
	G_VAR(VentanaScheduleTableSelection);
	G_VAR(VentanaScheduleTableMoveUp);
	G_VAR(VentanaScheduleTableMoveDown);
	G_VAR(VentanaScheduleTableNew);
	G_VAR(VentanaScheduleTableEdit);
	G_VAR(VentanaScheduleTableDelete);
	G_VAR(VentanaScheduleTableAccept);
	G_VAR(VentanaScheduleTableCancel);

	static void OnVentanaScheduleTableList_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);
	static void OnVentanaScheduleTableSelection_changed(GtkTreeSelection *widget, gpointer user_data);
	static void OnVentanaScheduleTableMoveUp_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaScheduleTableMoveDown_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaScheduleTableNew_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaScheduleTableEdit_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaScheduleTableDelete_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaScheduleTableAccept_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaScheduleTableCancel_clicked(GtkButton *button, gpointer user_data);

	void PrepareListCommands();

public:
	VentanaScheduleTable(GtkBuilder *builder, ldf *db, const char *schedule_table_name);
	virtual ~VentanaScheduleTable();

	ldfscheduletable *ShowModal(GObject *parent);

};

} /* namespace ui */

#endif /* UI_VENTANASCHEDULETABLE_H_ */
