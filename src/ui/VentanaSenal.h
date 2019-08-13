/*
 * VentanaSenal.h
 *
 *  Created on: 12 ago. 2019
 *      Author: iso9660
 */

#ifndef UI_VENTANASENAL_H_
#define UI_VENTANASENAL_H_

#include "tools.h"
#include "ldf.h"

using namespace tools;
using namespace lin;

namespace ui {

class VentanaSenal
{

private:
	GObject *handle;
	GtkBuilder *builder;
	ldf *db;
	const char *signal_name;

	G_VAR(VentanaSenalName);
	G_VAR(VentanaSenalBitSize);
	G_VAR(VentanaSenalDefaultValue);
	G_VAR(VentanaSenalPublisher);
	G_VAR(VentanaSenalSubscriberList);
	G_VAR(VentanaSenalSubscriberSelection);
	G_VAR(VentanaSenalSubscriberNew);
	G_VAR(VentanaSenalSubscriberEdit);
	G_VAR(VentanaSenalSubscriberDelete);
	G_VAR(VentanaSenalAccept);
	G_VAR(VentanaSenalCancel);

	void PrepareListSubscribers();
	void GetAllFreeSubscriberNodes(const char **nodes, int *count);

	// Signal events
	static void OnVentanaSenalPublisher_changed(GtkComboBoxText *widget, gpointer user_data);
	static void OnVentanaSenalSubscriberList_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);
	static void OnVentanaSenalSubscriberSelection_changed(GtkTreeSelection *widget, gpointer user_data);
	static void OnVentanaSenalSubscriberNew_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaSenalSubscriberEdit_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaSenalSubscriberDelete_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaSenalAccept_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaSenalCancel_clicked(GtkButton *button, gpointer user_data);


public:
	VentanaSenal(GtkBuilder *builder, ldf *db, const char *signal_name);
	virtual ~VentanaSenal();

	ldfsignal *ShowModal(GObject *parent);

};

} /* namespace ui */

#endif /* UI_VENTANASENAL_H_ */
