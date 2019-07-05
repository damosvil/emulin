/*
 * ldfnodeattributes.h
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFNODEATTRIBUTES_H_
#define LIN_LDFNODEATTRIBUTES_H_

#include <stdint.h>
#include <ldfcommon.h>
#include <ldfconfigurableframe.h>


namespace lin {

class ldfnodeattributes {

public:
	struct product_id_s
	{
		uint32_t supplier_id;
		uint32_t function_id;
		uint32_t variant;
	};

private:
	uint8_t *name;
	lin_protocol_version_e protocol;
	uint8_t configured_NAD;
	uint8_t initial_NAD;
	product_id_s product_id;
	uint8_t *response_error_signal_name;
	uint8_t *fault_state_signals[1000];
	uint32_t fault_state_signals_count;
	uint16_t P2_min;
	uint16_t ST_min;
	uint16_t N_As_timeout;
	uint16_t N_Cr_timeout;
	ldfconfigurableframe *configurable_frames[1000];
	uint16_t configurable_frames_count;

public:
	ldfnodeattributes(uint8_t *name);
	virtual ~ldfnodeattributes();

	void UpdateFromLdfStatement(uint8_t *statement);
	void AddConfigurableFrame(ldfconfigurableframe *frame);

};

} /* namespace lin */

#endif /* LIN_LDFNODEATTRIBUTES_H_ */
