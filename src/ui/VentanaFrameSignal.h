/*
 * VentanaFrameSignal.h
 *
 *  Created on: 22 ago. 2019
 *      Author: iso9660
 */

#ifndef UI_VENTANAFRAMESIGNAL_H_
#define UI_VENTANAFRAMESIGNAL_H_

#include "tools.h"
#include "ldf.h"

using namespace tools;
using namespace lin;


namespace ui {

class VentanaFrameSignal
{
public:
	struct frame_signals_raw_t
	{
		char *offset;
		char *name;
	};

private:
	GObject *handle;
	ldf *db;
	frame_signals_raw_t **frame_signals;
	int frame_signals_count;

	G_VAR(VentanaFrameSignalOffset);
	G_VAR(VentanaFrameSignalName);
	G_VAR(VentanaFrameSignalSize);
	G_VAR(VentanaFrameSignalAccept);
	G_VAR(VentanaFrameSignalCancel);

	// Signal events
	static void OnVentanaFrameSignalName_changed(GtkComboBoxText *widget, gpointer user_data);
	static void OnVentanaFrameSignalAccept_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaFrameSignalCancel_clicked(GtkButton *button, gpointer user_data);

public:
	VentanaFrameSignal(GtkBuilder *builder, ldf *db, char **signal_names, int signal_names_count, frame_signals_raw_t **frame_signals, int frame_signals_count);
	virtual ~VentanaFrameSignal();

	ldfframesignal *ShowModal(GObject *parent);

};

} /* namespace ui */

#endif /* UI_VENTANAFRAMESIGNAL_H_ */
