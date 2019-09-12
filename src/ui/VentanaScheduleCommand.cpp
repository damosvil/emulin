/*
 * VentanaCommandFrame.cpp
 *
 *  Created on: 6 sept. 2019
 *      Author: iso9660
 */

#include <VentanaScheduleCommand.h>

namespace ui {

VentanaScheduleCommand::VentanaScheduleCommand(GtkBuilder *builder, ldf *db, const char *command, const char *timeout)
{
	// Store data
	this->builder = builder;
	this->db = db;
	this->handle = gtk_builder_get_object(builder, "VentanaScheduleCommand");

	// Pin Widgets
	G_PIN(VentanaScheduleCommandType);
	G_PIN(VentanaScheduleCommandFrameName);
	G_PIN(VentanaScheduleCommandSlave);
	G_PIN(VentanaScheduleCommandTimeout);
	G_PIN(VentanaScheduleCommandDataRaw);
	G_PIN(VentanaScheduleCommandDataFrame);
	G_PIN(VentanaScheduleCommandDataCount);
	G_PIN(VentanaScheduleCommandAccept);
	G_PIN(VentanaScheduleCommandCancel);

	// Load data
	if (command != NULL)
	{
		ldfschedulecommand *c = ldfschedulecommand::FromStrCommand(db, Str(command), Str(timeout));

		// Type
		gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaScheduleCommandType), (char *)c->GetStrType());

		// Frame name
		EntrySet(g_VentanaScheduleCommandFrameName, c->GetAssignFrameName());

		delete c;
	}
	else
	{

	}

	// Connect signals
	G_CONNECT_INSTXT(VentanaScheduleCommandTimeout, INT5_EXPR);
	G_CONNECT_INSTXT(VentanaScheduleCommandDataCount, INT_1_4_EXPR);
	G_CONNECT(VentanaScheduleCommandType, changed);
	G_CONNECT(VentanaScheduleCommandSlave, changed);
	G_CONNECT(VentanaScheduleCommandAccept, clicked);
	G_CONNECT(VentanaScheduleCommandCancel, clicked);
}

VentanaScheduleCommand::~VentanaScheduleCommand()
{
	// Disconnect signals

}

ldfschedulecommand *VentanaScheduleCommand::ShowModal(GObject *parent)
{
	ldfschedulecommand *res = NULL;

	// Put this window always on top of parent
	gtk_window_set_transient_for(GTK_WINDOW(handle), GTK_WINDOW(parent));

	// Show dialog
	if (gtk_dialog_run(GTK_DIALOG(handle)))
	{
		// Create new command
		//res = new ldfschedulecommand();
	}
	gtk_widget_hide(GTK_WIDGET(handle));

	return res;
}

void VentanaScheduleCommand::OnVentanaScheduleCommandType_changed(GtkComboBoxText *widget, gpointer user_data)
{

}

void VentanaScheduleCommand::OnVentanaScheduleCommandSlave_changed(GtkComboBoxText *widget, gpointer user_data)
{

}

void VentanaScheduleCommand::OnVentanaScheduleCommandAccept_clicked(GtkButton *button, gpointer user_data)
{
	VentanaScheduleCommand *v = (VentanaScheduleCommand *)user_data;
	uint32_t timeout = EntryGetInt(v->g_VentanaScheduleCommandTimeout);
	uint32_t min_timeout = 10 * 19200 / v->db->GetLinSpeed();	// Laterally thinking, at 19200bps min timeout for a 8 byte payload frame is 10ms, so if slower time shall be longer

	// Timeout
	if (timeout < min_timeout)
	{
		// Schedule table name in use
		ShowErrorMessageBox(v->handle, "Timeout shall be longer than %d ms", min_timeout);
		return;
	}

	// Return true
	gtk_dialog_response(GTK_DIALOG(v->handle), true);
}

void VentanaScheduleCommand::OnVentanaScheduleCommandCancel_clicked(GtkButton *button, gpointer user_data)
{
	VentanaScheduleCommand *v = (VentanaScheduleCommand *)user_data;

	// Return false
	gtk_dialog_response(GTK_DIALOG(v->handle), false);
}


} /* namespace ui */
