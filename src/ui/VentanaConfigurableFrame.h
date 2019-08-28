/*
 * VentanaConfigurableFrame.h
 *
 *  Created on: 28 ago. 2019
 *      Author: iso9660
 */

#ifndef UI_VENTANACONFIGURABLEFRAME_H_
#define UI_VENTANACONFIGURABLEFRAME_H_

#include "tools.h"
#include "ldf.h"

using namespace tools;
using namespace lin;


namespace ui {

class VentanaConfigurableFrame
{

public:
	struct configurable_frame_raw_t
	{
		char *id;
		char *name;
	};

private:
	GObject *handle;
	ldf *db;
	configurable_frame_raw_t *config_frames;
	int config_frames_count;

	// Widgets
	G_VAR(VentanaConfigurableFrameID);
	G_VAR(VentanaConfigurableFrameName);
	G_VAR(VentanaConfigurableFrameAccept);
	G_VAR(VentanaConfigurableFrameCancel);

	// Signal events
	static void OnVentanaConfigurableFrameAccept_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaConfigurableFrameCancel_clicked(GtkButton *button, gpointer user_data);

public:
	VentanaConfigurableFrame(GtkBuilder *builder, ldf *db, char **frame_names, int frame_names_count, configurable_frame_raw_t *config_frames, int config_frames_count);
	virtual ~VentanaConfigurableFrame();

	ldfconfigurableframe *ShowModal(GObject *parent);

};

} /* namespace ui */

#endif /* UI_VENTANACONFIGURABLEFRAME_H_ */
