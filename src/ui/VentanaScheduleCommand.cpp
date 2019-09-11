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

	// Connect signals
}

VentanaScheduleCommand::~VentanaScheduleCommand()
{
	// TODO Auto-generated destructor stub
}

ldfschedulecommand *VentanaScheduleCommand::ShowModal(GObject *parent)
{

}



} /* namespace ui */
