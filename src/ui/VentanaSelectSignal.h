/*
 * VentanaSelectSignal.h
 *
 *  Created on: 13 ago. 2019
 *      Author: iso9660
 */

#ifndef UI_VENTANASELECTSIGNAL_H_
#define UI_VENTANASELECTSIGNAL_H_

#include "tools.h"
#include "ldf.h"

using namespace tools;
using namespace lin;


namespace ui {

class VentanaSelectSignal
{

private:
	GObject *handle;

	G_VAR(VentanaSelectSignalName);
	G_VAR(VentanaSelectSignalAccept);
	G_VAR(VentanaSelectSignalCancel);

	// Signal events
	static void OnVentanaSelectSignalAccept_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaSelectSignalCancel_clicked(GtkButton *button, gpointer user_data);

public:
	VentanaSelectSignal(GtkBuilder *builder, const char *signal_name, const char **signal_names, int signal_names_count);
	virtual ~VentanaSelectSignal();

	const char *ShowModal(GObject *parent);

};

} /* namespace ui */

#endif /* UI_VENTANASELECTSIGNAL_H_ */
