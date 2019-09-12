/*
 * VentanaScheduleTable.cpp
 *
 *  Created on: 6 sept. 2019
 *      Author: iso9660
 */

#include <VentanaScheduleTable.h>
#include <VentanaScheduleCommand.h>


namespace ui
{

VentanaScheduleTable::VentanaScheduleTable(GtkBuilder *builder, ldf *db, const char *schedule_table_name)
{
	// Store data
	this->builder = builder;
	this->db = db;
	this->schedule_table_name = schedule_table_name;
	this->handle = gtk_builder_get_object(builder, "VentanaScheduleTable");

	// Pin widgets
	G_PIN(VentanaScheduleTableName);
	G_PIN(VentanaScheduleTableList);
	G_PIN(VentanaScheduleTableSelection);
	G_PIN(VentanaScheduleTableMoveUp);
	G_PIN(VentanaScheduleTableMoveDown);
	G_PIN(VentanaScheduleTableNew);
	G_PIN(VentanaScheduleTableEdit);
	G_PIN(VentanaScheduleTableDelete);
	G_PIN(VentanaScheduleTableAccept);
	G_PIN(VentanaScheduleTableCancel);

	// Prepare lists
	PrepareListCommands();

	// Clear schedule commands list
	GtkTreeIter it;
	GtkTreeView *v = GTK_TREE_VIEW(g_VentanaScheduleTableList);
	GtkListStore *s = GTK_LIST_STORE(gtk_tree_view_get_model(v));
	gtk_list_store_clear(s);

	// Load data
	if (schedule_table_name != NULL)
	{
		ldfscheduletable *t = db->GetScheduleTableByName(Str(schedule_table_name));

		// Table name
		EntrySet(g_VentanaScheduleTableName, schedule_table_name);

		// Load list store
		for (uint32_t i = 0; i < t->GetCommandsCount(); i++)
		{
			ldfschedulecommand *c = t->GetCommandByIndex(i);

			// Append a new list item
			gtk_list_store_append(s, &it);

			// Command
			gtk_list_store_set(s, &it, 0, c->GetUiCommandText(db), -1);

			// Timeout
			gtk_list_store_set(s, &it, 1, GetStrPrintf("%d ms", c->GetTimeoutMs()), -1);
		}
	}
	else
	{
		// Table name
		EntrySet(g_VentanaScheduleTableName, "schedule");
	}

	// Connect signals
	G_CONNECT_INSTXT(VentanaScheduleTableName, NAME_EXPR);

	// Connect lists
	G_CONNECT(VentanaScheduleTableSelection, changed);
	G_CONNECT(VentanaScheduleTableList, row_activated);

	// Connect buttons
	G_CONNECT(VentanaScheduleTableMoveUp, clicked);
	G_CONNECT(VentanaScheduleTableMoveDown, clicked);
	G_CONNECT(VentanaScheduleTableNew, clicked);
	G_CONNECT(VentanaScheduleTableEdit, clicked);
	G_CONNECT(VentanaScheduleTableDelete, clicked);
	G_CONNECT(VentanaScheduleTableAccept, clicked);
	G_CONNECT(VentanaScheduleTableCancel, clicked);
}

VentanaScheduleTable::~VentanaScheduleTable()
{
	// Disconnect signals
	G_DISCONNECT_FUNC(VentanaScheduleTableName, EditableInsertValidator);

	// Connect lists
	G_DISCONNECT_DATA(VentanaScheduleTableSelection, this);
	G_DISCONNECT_DATA(VentanaScheduleTableList, this);

	// Connect buttons
	G_DISCONNECT_DATA(VentanaScheduleTableMoveUp, this);
	G_DISCONNECT_DATA(VentanaScheduleTableMoveDown, this);
	G_DISCONNECT_DATA(VentanaScheduleTableNew, this);
	G_DISCONNECT_DATA(VentanaScheduleTableEdit, this);
	G_DISCONNECT_DATA(VentanaScheduleTableDelete, this);
	G_DISCONNECT_DATA(VentanaScheduleTableAccept, this);
	G_DISCONNECT_DATA(VentanaScheduleTableCancel, this);
}

void VentanaScheduleTable::OnVentanaScheduleTableList_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
	VentanaScheduleTable *v = (VentanaScheduleTable *)user_data;

	gtk_widget_activate(GTK_WIDGET(v->g_VentanaScheduleTableEdit));
}

void VentanaScheduleTable::OnVentanaScheduleTableSelection_changed(GtkTreeSelection *widget, gpointer user_data)
{
	VentanaScheduleTable *v = (VentanaScheduleTable *)user_data;

	bool enable = gtk_tree_selection_count_selected_rows(widget) == 1;
	WidgetEnable(v->g_VentanaScheduleTableEdit, enable);
	WidgetEnable(v->g_VentanaScheduleTableDelete, enable);
}

void VentanaScheduleTable::OnVentanaScheduleTableMoveUp_clicked(GtkButton *button, gpointer user_data)
{

}

void VentanaScheduleTable::OnVentanaScheduleTableMoveDown_clicked(GtkButton *button, gpointer user_data)
{

}

void VentanaScheduleTable::OnVentanaScheduleTableNew_clicked(GtkButton *button, gpointer user_data)
{

}

void VentanaScheduleTable::OnVentanaScheduleTableEdit_clicked(GtkButton *button, gpointer user_data)
{

}

void VentanaScheduleTable::OnVentanaScheduleTableDelete_clicked(GtkButton *button, gpointer user_data)
{

}

void VentanaScheduleTable::OnVentanaScheduleTableAccept_clicked(GtkButton *button, gpointer user_data)
{
	VentanaScheduleTable *v = (VentanaScheduleTable *)user_data;
	const char *new_schedule_table_name = EntryGetStr(v->g_VentanaScheduleTableName);

	// Validate name
	if (v->schedule_table_name == NULL && v->db->GetScheduleTableByName(Str(new_schedule_table_name)) != NULL)
	{
		// Schedule table name in use
		ShowErrorMessageBox(v->handle, "Schedule table name identifier '%s' already in use.", new_schedule_table_name);;
		return;
	}
	else if (v->schedule_table_name != NULL && !Same(v->schedule_table_name, new_schedule_table_name) && v->db->GetScheduleTableByName(Str(new_schedule_table_name)) != NULL)
	{
		// Schedule table name in use
		ShowErrorMessageBox(v->handle, "Schedule table name identifier '%s' already in use.", new_schedule_table_name);;
		return;
	}

	// Return true
	gtk_dialog_response(GTK_DIALOG(v->handle), true);
}

void VentanaScheduleTable::OnVentanaScheduleTableCancel_clicked(GtkButton *button, gpointer user_data)
{
	VentanaScheduleTable *v = (VentanaScheduleTable *)user_data;

	// Return false
	gtk_dialog_response(GTK_DIALOG(v->handle), false);
}

void VentanaScheduleTable::PrepareListCommands()
{
	const char *columns[] = { "Command", "Timeout", NULL };

	TreeViewPrepare(g_VentanaScheduleTableList, columns);

	// Disable edit and delete buttons
	WidgetEnable(g_VentanaScheduleTableEdit, false);
	WidgetEnable(g_VentanaScheduleTableDelete, false);
}

ldfscheduletable *VentanaScheduleTable::ShowModal(GObject *parent)
{
	ldfscheduletable *res = NULL;

	// Put this window always on top of parent
	gtk_window_set_transient_for(GTK_WINDOW(handle), GTK_WINDOW(parent));

	// Show dialog
	if (gtk_dialog_run(GTK_DIALOG(handle)))
	{
		// Name
		res = new ldfscheduletable(Str(EntryGetStr(g_VentanaScheduleTableName)));

	}
	gtk_widget_hide(GTK_WIDGET(handle));

	return res;
}


} /* namespace ui */
