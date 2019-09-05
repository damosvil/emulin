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

private:
	uint8_t *frame_name;
	uint16_t timeout;

public:
	ldfschedulecommand(uint8_t *frame_name, uint16_t timeout);
	virtual ~ldfschedulecommand();

	static ldfschedulecommand *FromLdfStatement(uint8_t *statement);

	uint8_t *GetName();
	uint16_t GetTimeoutMs();
	void UpdateName(const uint8_t *old_name, const uint8_t *new_name);

	void ValidateUnicity(uint8_t *schedule_table, ldfschedulecommand *command, uint8_t **validation_messages, uint32_t *validation_messages_count);

};

} /* namespace lin */

#endif /* LIN_LDFSCHEDULECOMMAND_H_ */
