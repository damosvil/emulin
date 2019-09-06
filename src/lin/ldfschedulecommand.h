/*
 * ldfschedulecommand.h
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFSCHEDULECOMMAND_H_
#define LIN_LDFSCHEDULECOMMAND_H_

#include <stdint.h>


namespace lin {

class ldfschedulecommand {

public:
	typedef enum ldfschedulecommandtype_e
	{
		LDF_SCMD_TYPE_UnconditionalFrame = 0,
		LDF_SCMD_TYPE_MarterReq = 1,
		LDF_SCMD_TYPE_SlaveResp = 2,
		LDF_SCMD_TYPE_AssignNAD = 3,
		LDF_SCMD_TYPE_DataDump = 4,
		LDF_SCMD_TYPE_SaveConfig = 5,
		LDF_SCMD_TYPE_FreeFormat = 6,
		LDF_SCMD_TYPE_AssignFrameIdRange = 7,
		LDF_SCMD_TYPE_AssignFrameId = 8
	} ldfschedulecommandtype_t;

private:
	ldfschedulecommandtype_t type;
	uint8_t *frame_name;
	uint16_t timeout;
	uint8_t *slave;
	uint8_t *data;
	uint8_t *assign_frame;

public:
	ldfschedulecommand(ldfschedulecommandtype_t, uint8_t *frame_name, uint16_t timeout, uint8_t *slave, uint8_t *data, uint8_t *assign_frame);
	virtual ~ldfschedulecommand();

	static ldfschedulecommand *FromLdfStatement(uint8_t *statement);

	uint8_t *GetName();
	uint16_t GetTimeoutMs();
	void UpdateName(const uint8_t *old_name, const uint8_t *new_name);

	void ValidateUnicity(uint8_t *schedule_table, ldfschedulecommand *command, uint8_t **validation_messages, uint32_t *validation_messages_count);

};

} /* namespace lin */

#endif /* LIN_LDFSCHEDULECOMMAND_H_ */
