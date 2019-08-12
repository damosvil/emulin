/*
 * VentanaNodoEsclavo.h
 *
 *  Created on: 7 ago. 2019
 *      Author: iso9660
 */

#ifndef UI_VENTANANODOESCLAVO_H_
#define UI_VENTANANODOESCLAVO_H_

#include "tools.h"
#include <gtk/gtk.h>
#include <ldf.h>

using namespace lin;

namespace ui {

class VentanaNodoEsclavo {

private:
	GObject *handle;
	ldf *db;
	uint8_t *slave_name;

	G_VAR(VentanaNodoEsclavoName);
	G_VAR(VentanaNodoEsclavoProtocolVersion);
	G_VAR(VentanaNodoEsclavoInitialNAD);
	G_VAR(VentanaNodoEsclavoConfiguredNAD);
	G_VAR(VentanaNodoEsclavoSupplierID);
	G_VAR(VentanaNodoEsclavoFunctionID);
	G_VAR(VentanaNodoEsclavoVariant);
	G_VAR(VentanaNodoEsclavoResponseErrorSignal);
	G_VAR(VentanaNodoEsclavoP2_min);
	G_VAR(VentanaNodoEsclavoST_min);
	G_VAR(VentanaNodoEsclavoN_As_timeout);
	G_VAR(VentanaNodoEsclavoN_Cr_timeout);
	G_VAR(VentanaNodoEsclavoConfigFrameList);
	G_VAR(VentanaNodoEsclavoConfigFrameNew);
	G_VAR(VentanaNodoEsclavoConfigFrameEdit);
	G_VAR(VentanaNodoEsclavoConfigFrameDelete);
	G_VAR(VentanaNodoEsclavoAccept);
	G_VAR(VentanaNodoEsclavoCancel);

	// Signal events
	static void OnVentanaNodoEsclavoConfigFrameNew_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaNodoEsclavoConfigFrameEdit_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaNodoEsclavoConfigFrameDelete_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaNodoEsclavoAccept_clicked(GtkButton *button, gpointer user_data);
	static void OnVentanaNodoEsclavoCancel_clicked(GtkButton *button, gpointer user_data);

public:
	VentanaNodoEsclavo(GtkBuilder *builder, ldf *db, uint8_t *slave_name);
	virtual ~VentanaNodoEsclavo();

	ldfnodeattributes *ShowModal(GObject *parent);

};

} /* namespace ui */

#endif /* UI_VENTANANODOESCLAVO_H_ */
