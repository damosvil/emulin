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
#include <ldfnode.h>
#include <ldfframe.h>
#include <ldfconfigurableframe.h>


namespace lin {

class ldfnodeattributes {

public:
	struct product_id_s
	{
		uint16_t supplier_id;
		uint16_t function_id;
		uint8_t variant;
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
	void ValidateNode(ldfnode **slaves, uint32_t slaves_count, uint8_t **validation_messages, uint32_t *validation_messages_count);
	void ValidateUnicity(ldfnodeattributes *attributes, uint8_t **validation_messages, uint32_t *validation_messages_count);
	void ValidateFrames(ldfframe **frames, uint32_t frames_count, uint8_t **validation_messages, uint32_t *validation_messages_count);

	uint8_t *GetName();
	lin_protocol_version_e GetProtocolVersion();
	uint8_t GetInitialNAD();
	uint8_t GetConfiguredNAD();
	uint16_t GetSupplierID();
	uint16_t GetFunctionID();
	uint8_t GetVariant();
	uint16_t GetP2_min();
	uint16_t GetST_min();
	uint16_t GetN_As_timeout();
	uint16_t GetN_Cr_timeout();
	uint8_t *GetResponseErrorSignalName();
	ldfconfigurableframe *GetConfigurableFrame(uint32_t ix);
	uint16_t GetConfigurableFramesCount();

	void SetProtocolVersion(lin_protocol_version_e v);
	void SetInitialNAD(uint8_t v);
	void SetConfiguredNAD(uint8_t v);
	void SetSupplierID(uint16_t v);
	void SetFunctionID(uint16_t v);
	void SetVariant(uint8_t v);
	void SetP2_min(uint16_t v);
	void SetST_min(uint16_t v);
	void SetN_As_timeout(uint16_t v);
	void SetN_Cr_timeout(uint16_t v);
	void SetResponseErrorSignalName(uint8_t *v);

	bool NameIs(uint8_t *name);

};

} /* namespace lin */

#endif /* LIN_LDFNODEATTRIBUTES_H_ */
