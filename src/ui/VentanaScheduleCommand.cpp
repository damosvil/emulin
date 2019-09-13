/*howo
 * VentanaCommandFrame.cpp
 *
 *  Created on: 6 sept. 2019
 *      Author: iso9660
 */

#include <VentanaScheduleCommand.h>
#include <ui.h>

namespace ui
{

VentanaScheduleCommand::VentanaScheduleCommand(GtkBuilder *builder, ldf *db, const char *command, const char *timeout)
{
	// Store data
	this->builder = builder;
	this->db = db;
	this->handle = gtk_builder_get_object(builder, "VentanaScheduleCommand");

	// Pin Widgets
	G_PIN(VentanaScheduleCommandTypeLabel);
	G_PIN(VentanaScheduleCommandFrameNameLabel);
	G_PIN(VentanaScheduleCommandSlaveLabel);
	G_PIN(VentanaScheduleCommandTimeoutLabel);
	G_PIN(VentanaScheduleCommandDataRawLabel);
	G_PIN(VentanaScheduleCommandDataFrameLabel);
	G_PIN(VentanaScheduleCommandDataCountLabel);
	G_PIN(VentanaScheduleCommandType);
	G_PIN(VentanaScheduleCommandFrameName);
	G_PIN(VentanaScheduleCommandSlave);
	G_PIN(VentanaScheduleCommandTimeout);
	G_PIN(VentanaScheduleCommandDataRaw);
	G_PIN(VentanaScheduleCommandDataFrame);
	G_PIN(VentanaScheduleCommandDataCount);
	G_PIN(VentanaScheduleCommandAccept);
	G_PIN(VentanaScheduleCommandCancel);

	// Load slave list and select first one by default
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(g_VentanaScheduleCommandSlave));
	for (uint32_t i = 0; i < db->GetSlaveNodesCount(); i++)
		gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(g_VentanaScheduleCommandSlave), (char *)db->GetSlaveNodeByIndex(i)->GetName(), (char *)db->GetSlaveNodeByIndex(i)->GetName());
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaScheduleCommandSlave), (char *)db->GetSlaveNodeByIndex(0)->GetName());

	// Load data
	if (command != NULL)
	{
		ldfschedulecommand *c = ldfschedulecommand::FromStrCommand(db, Str(command), Str(timeout));

		// Type
		gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaScheduleCommandType), (char *)c->GetStrType());

		// Frame name
		gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(g_VentanaScheduleCommandSlave));
		if (c->GetType() == ldfschedulecommand::LDF_SCMD_TYPE_UnconditionalFrame)
		{
			for (uint32_t i = 0; i < db->GetFramesCount(); i++)
				gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(g_VentanaScheduleCommandSlave), (char *)db->GetFrameByIndex(i)->GetName(), (char *)db->GetFrameByIndex(i)->GetName());
			gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaScheduleCommandSlave), (char *)db->GetFrameByIndex(0)->GetName());
			gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaScheduleCommandSlave), (char *)c->GetFrameName());
		}
		else
		{
			gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(g_VentanaScheduleCommandSlave), (char *)c->GetStrType(), (char *)c->GetStrType());
			gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaScheduleCommandSlave), (char *)c->GetStrType());
		}

		// Slave
		if (c->GetSlaveName() != NULL)
			gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaScheduleCommandType), (char *)c->GetSlaveName());

		// Timeout
		EntrySet(g_VentanaScheduleCommandTimeout, GetStrPrintf("%d", c->GetTimeoutMs()));

		// Raw data
		EntrySet(g_VentanaScheduleCommandDataRaw, c->GetStrRawData());

		// Data frame depends on the slave
		ldfnodeattributes *aa = db->GetSlaveNodeAttributesByName(c->GetSlaveName());
		if (aa != NULL && aa->GetConfigurableFramesCount() > 0)
		{
			// Load slave frame list with all slave configurable_frames and select the fist one
			gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(g_VentanaScheduleCommandDataFrame));
			for (uint32_t i = 0; i < aa->GetConfigurableFramesCount(); i++)
				gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(g_VentanaScheduleCommandDataFrame), (char*)aa->GetConfigurableFrame(i)->GetName(), (char*)aa->GetConfigurableFrame(i)->GetName());
			gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaScheduleCommandDataFrame), (char*)aa->GetConfigurableFrame(0)->GetName());

			// Set default AssignFrameId and frame count
			gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaScheduleCommandDataFrame), (char*)c->GetAssignFrameIdName());
			EntrySet(g_VentanaScheduleCommandDataCount, "1");

			// Select the frame index and data count depends on the frame
			if (c->GetType() == ldfschedulecommand::LDF_SCMD_TYPE_AssignFrameIdRange)
			{
				ldfframe *frame = db->GetFrameByPid(c->GetAssignFrameIdRangePid());
				if (frame != NULL)
				{
					gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaScheduleCommandDataFrame), (char*)frame->GetName());
					EntrySet(g_VentanaScheduleCommandDataCount, GetStrPrintf("%d", c->GetAssignFrameIdRangeCount()));
				}
			}
		}

		// Delete schedule command
		delete c;
	}
	else
	{
		// Type
		gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaScheduleCommandType), "UnconditionalFrame");

		// Frame name
		for (uint32_t i = 0; i < db->GetFramesCount(); i++)
			gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(g_VentanaScheduleCommandSlave), (char *)db->GetFrameByIndex(i)->GetName(), (char *)db->GetFrameByIndex(i)->GetName());
		gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaScheduleCommandSlave), (char *)db->GetFrameByIndex(0)->GetName());

		// Timeout
		EntrySet(g_VentanaScheduleCommandTimeout, "10");

		// Raw data
		EntrySet(g_VentanaScheduleCommandDataRaw, "");

		// Clear data frame list
		gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(g_VentanaScheduleCommandDataFrame));

		// Data count
		EntrySet(g_VentanaScheduleCommandDataCount, "1");
	}

	// React as if type changed
	this->OnVentanaScheduleCommandType_changed(GTK_COMBO_BOX_TEXT(g_VentanaScheduleCommandType), this);

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
	G_DISCONNECT_FUNC(VentanaScheduleCommandTimeout, EditableInsertValidator);
	G_DISCONNECT_FUNC(VentanaScheduleCommandDataCount, EditableInsertValidator);
	G_DISCONNECT_DATA(VentanaScheduleCommandType, this);
	G_DISCONNECT_DATA(VentanaScheduleCommandSlave, this);
	G_DISCONNECT_DATA(VentanaScheduleCommandAccept, this);
	G_DISCONNECT_DATA(VentanaScheduleCommandCancel, this);
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

void VentanaScheduleCommand::UpdateLayout()
{
	const char *type = gtk_combo_box_get_active_id(GTK_COMBO_BOX(g_VentanaScheduleCommandType));

	bool b_ShowSlave = false;
	bool b_ShowDataRaw = false;
	bool b_ShowDataFrame = false;
	bool b_ShowDataCount = false;

	if (StrEq(type, "AssignNAD"))
	{
		b_ShowSlave = true;
	}
	else if (StrEq(type, "DataDump"))
	{
		b_ShowSlave = true;
		b_ShowDataRaw = true;
	}
	else if (StrEq(type, "SaveConfiguration"))
	{
		b_ShowSlave = true;
	}
	else if (StrEq(type, "FreeFormat"))
	{
		b_ShowDataRaw = true;
	}
	else if (StrEq(type, "AssignFrameIdRange"))
	{
		b_ShowSlave = true;
		b_ShowDataFrame = true;
		b_ShowDataCount = true;
	}
	else if (StrEq(type, "AssignFrameId"))
	{
		b_ShowSlave = true;
		b_ShowDataFrame = true;
	}

	WidgetShow(g_VentanaScheduleCommandSlave 	, b_ShowSlave);
	WidgetShow(g_VentanaScheduleCommandSlave 	, b_ShowSlave);
	WidgetShow(g_VentanaScheduleCommandDataRaw 	, b_ShowDataRaw);
	WidgetShow(g_VentanaScheduleCommandDataRaw 	, b_ShowDataRaw);
	WidgetShow(g_VentanaScheduleCommandDataFrame, b_ShowDataFrame);
	WidgetShow(g_VentanaScheduleCommandDataFrame, b_ShowDataFrame);
	WidgetShow(g_VentanaScheduleCommandDataCount, b_ShowDataCount);
	WidgetShow(g_VentanaScheduleCommandDataCount, b_ShowDataCount);
}

void VentanaScheduleCommand::OnVentanaScheduleCommandType_changed(GtkComboBoxText *widget, gpointer user_data)
{
	VentanaScheduleCommand *v = (VentanaScheduleCommand *)user_data;
	const char *type = gtk_combo_box_get_active_id(GTK_COMBO_BOX(v->g_VentanaScheduleCommandType));

	// Update layout
	v->UpdateLayout();

	if (StrEq(type, "DataDump"))
	{
		if (!RegExprCheck(EntryGetStr(v->g_VentanaScheduleCommandDataRaw), RAW_DATA_5))
			EntrySet(v->g_VentanaScheduleCommandDataRaw, "0x00 0x00 0x00 0x00 0x00");
	}
	else if (StrEq(type, "FreeFormat"))
	{
		if (!RegExprCheck(EntryGetStr(v->g_VentanaScheduleCommandDataRaw), RAW_DATA_8))
			EntrySet(v->g_VentanaScheduleCommandDataRaw, "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00");
	}
}

void VentanaScheduleCommand::OnVentanaScheduleCommandSlave_changed(GtkComboBoxText *widget, gpointer user_data)
{
	VentanaScheduleCommand *v = (VentanaScheduleCommand *)user_data;
	const char *slave = gtk_combo_box_get_active_id(GTK_COMBO_BOX(v->g_VentanaScheduleCommandSlave));

	// Reload slave frame list with all slave configurable_frames and select the fist one
	ldfnodeattributes *aa = v->db->GetSlaveNodeAttributesByName(Str(slave));
	if (aa != NULL && aa->GetConfigurableFramesCount() > 0)
	{
		gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(v->g_VentanaScheduleCommandDataFrame));
		for (uint32_t i = 0; i < aa->GetConfigurableFramesCount(); i++)
			gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(v->g_VentanaScheduleCommandDataFrame), (char*)aa->GetConfigurableFrame(i)->GetName(), (char*)aa->GetConfigurableFrame(i)->GetName());
		gtk_combo_box_set_active_id(GTK_COMBO_BOX(v->g_VentanaScheduleCommandDataFrame), (char*)aa->GetConfigurableFrame(0)->GetName());
	}

	// Set AssignFrameId count to 1
	EntrySet(v->g_VentanaScheduleCommandDataCount, "1");
}

void VentanaScheduleCommand::OnVentanaScheduleCommandAccept_clicked(GtkButton *button, gpointer user_data)
{
	VentanaScheduleCommand *v = (VentanaScheduleCommand *)user_data;
	uint32_t min_timeout = 10 * 19200 / v->db->GetLinSpeed();	// Laterally thinking, at 19200bps min timeout for a 8 byte payload frame is 10ms, so if slower time shall be longer
	const char *str_type = gtk_combo_box_get_active_id(GTK_COMBO_BOX(v->g_VentanaScheduleCommandType));
	const char *str_slave = gtk_combo_box_get_active_id(GTK_COMBO_BOX(v->g_VentanaScheduleCommandSlave));
	const char *str_assign_frame = gtk_combo_box_get_active_id(GTK_COMBO_BOX(v->g_VentanaScheduleCommandDataFrame));
	const char *str_data = EntryGetStr(v->g_VentanaScheduleCommandDataRaw);
	uint32_t timeout = EntryGetInt(v->g_VentanaScheduleCommandTimeout);
	uint32_t data_count = EntryGetInt(v->g_VentanaScheduleCommandDataCount);

	// Timeout
	if (timeout < min_timeout)
	{
		// Schedule table name in use
		ShowErrorMessageBox(v->handle, "Timeout shall be longer than %d ms", min_timeout);
		return;
	}

	else if (StrEq(str_type, "DataDump"))
	{
		if (!RegExprCheck(str_data, RAW_DATA_5))
		{
			ShowErrorMessageBox(v->handle, "DataDump requires RawData field matching the format '0x00 0x00 0x00 0x00 0x00'");
			return;
		}
	}
	else if (StrEq(str_type, "FreeFormat"))
	{
		if (!RegExprCheck(str_data, RAW_DATA_8))
		{
			ShowErrorMessageBox(v->handle, "FreeFormat requires RawData field matching the format '0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00'");
			return;
		}
	}
	else if (StrEq(str_type, "AssignFrameIdRange"))
	{
		if (data_count == 0 || data_count > 4)
		{
			ShowErrorMessageBox(v->handle, "Configurable frames count shall be between 1 and 4");
			return;
		}

		ldfnodeattributes *a = v->db->GetSlaveNodeAttributesByName(Str(str_slave));
		for (uint32_t i = 0; i < a->GetConfigurableFramesCount(); i++)
		{
			if (StrEq(str_assign_frame, a->GetName()))
			{
				if ((a->GetConfigurableFramesCount() - i) < data_count)
				{
					ShowErrorMessageBox(v->handle, "There are only %d configurable frames count from specified index.", a->GetConfigurableFramesCount() - i);
					return;
				}
				break;
			}
		}
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
