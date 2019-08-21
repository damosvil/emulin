/*
 * VentanaFrame.h
 *
 *  Created on: 21 ago. 2019
 *      Author: iso9660
 */

#ifndef UI_VENTANAFRAME_H_
#define UI_VENTANAFRAME_H_

#include "tools.h"
#include "ldf.h"

using namespace tools;
using namespace lin;

namespace ui {

class VentanaFrame
{

private:
	GObject *handle;
	GtkBuilder *builder;
	ldf *db;
	const char *frame_name;

	G_VAR(VentanaFrameName);
	G_VAR(VentanaFrameID);
	G_VAR(VentanaFrameSize);
	G_VAR(VentanaFramePublisher);
	G_VAR(VentanaFrameSignalsList);
	G_VAR(VentanaFrameSignalsSelection);
	G_VAR(VentanaFrameSignalsNew);
	G_VAR(VentanaFrameSignalsEdit);
	G_VAR(VentanaFrameSignalsDelete);
	G_VAR(VentanaFrameAccept);
	G_VAR(VentanaFrameCancel);

	void PrepareListSignals();

	// Signal events
	static void OnVentanaFramePublisher_changed(GtkComboBoxText *widget, gpointer user_data);
	static void OnVentanaFrameSignalsList_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);
	static void OnVentanaFrameSignalsSelection_changed(GtkTreeSelection *widget, gpointer user_data);
	static void OnVentanaFrameSignalsNew_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaFrameSignalsEdit_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaFrameSignalsDelete_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaFrameAccept_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaFrameCancel_clicked(GtkButton *button, gpointer user_data);

public:
	VentanaFrame(GtkBuilder *builder, ldf *db, const char *frame_name);
	virtual ~VentanaFrame();

	ldfframe *ShowModal(GObject *parent);

};

} /* namespace ui */

#endif /* UI_VENTANAFRAME_H_ */
