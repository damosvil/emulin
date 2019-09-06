/*
 * VentanaCommandFrame.h
 *
 *  Created on: 6 sept. 2019
 *      Author: iso9660
 */

#ifndef UI_VENTANASCHEDULECOMMAND_H_
#define UI_VENTANASCHEDULECOMMAND_H_

#include "tools.h"
#include "ldf.h"

using namespace tools;
using namespace lin;


namespace ui {

class VentanaScheduleCommand
{

private:
	GtkBuilder *builder;
	ldf *db;

public:
	VentanaScheduleCommand(GtkBuilder *builder, ldf *db);
	virtual ~VentanaScheduleCommand();

};

} /* namespace ui */

#endif /* UI_VENTANASCHEDULECOMMAND_H_ */
