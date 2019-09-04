/*
 * VentanaSelectSignal.h
 *
 *  Created on: 13 ago. 2019
 *      Author: iso9660
 */

#ifndef UI_VENTANASELECTNODE_H_
#define UI_VENTANASELECTNODE_H_

#include "tools.h"
#include "ldf.h"

using namespace tools;
using namespace lin;


namespace ui {

class VentanaSelectNode
{

private:
	GObject *handle;

	G_VAR(VentanaSelectNodeName);
	G_VAR(VentanaSelectNodeAccept);
	G_VAR(VentanaSelectNodeCancel);

	// Signal events
	static void OnVentanaSelectNodeAccept_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaSelectNodeCancel_clicked(GtkButton *button, gpointer user_data);

public:
	VentanaSelectNode(GtkBuilder *builder, const char **node_names, int node_names_count);
	virtual ~VentanaSelectNode();

	const char *ShowModal(GObject *parent);

};

} /* namespace ui */

#endif /* UI_VENTANASELECTNODE_H_ */
