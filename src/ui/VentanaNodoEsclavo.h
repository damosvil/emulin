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
	ldf *db;
	uint8_t *slave_name;

	G_VAR(Name);
	G_VAR(ProtocolVersion);
	G_VAR(InitialNAD);
	G_VAR(ConfiguredNAD);
	G_VAR(SupplierID);
	G_VAR(FunctionID);
	G_VAR(Variant);
	G_VAR(ResponseErrorSignal);
	G_VAR(P2_min);
	G_VAR(ST_min);
	G_VAR(N_As_timeout);
	G_VAR(N_Cr_timeout);
	G_VAR(ConfigFrameList);
	G_VAR(ConfigFrameNew);
	G_VAR(ConfigFrameEdit);
	G_VAR(ConfigFrameDelete);
	G_VAR(Accept);
	G_VAR(Cancel);

public:
	VentanaNodoEsclavo(ldf *db,	uint8_t *slave_name);
	virtual ~VentanaNodoEsclavo();

};

} /* namespace ui */

#endif /* UI_VENTANANODOESCLAVO_H_ */
