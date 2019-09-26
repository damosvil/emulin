/*
 * ldfscheduletable.h
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFSCHEDULETABLE_H_
#define LIN_LDFSCHEDULETABLE_H_

#include <stdint.h>
#include <ldfframe.h>
#include <ldfschedulecommand.h>


namespace lin
{

class ldfscheduletable {

private:
	uint8_t *name;

	// Schedule commands
	ldfschedulecommand *commands[1000];
	uint16_t commands_count;

public:
	ldfscheduletable(const uint8_t *name);
	virtual ~ldfscheduletable();

	uint8_t *GetName();
	ldfschedulecommand *GetCommandByIndex(uint32_t ix);
	uint16_t GetCommandsCount();
	void UpdateCommandsFrameName(const uint8_t *old_name, const uint8_t *new_name);
	void UpdateCommandsSlaveName(const uint8_t *old_name, const uint8_t *new_name);
	void DeleteCommandsByFrameName(const uint8_t *name);
	void DeleteCommandsBySlaveName(const uint8_t *name);
	void DeleteCommandByIndex(uint32_t ix);

	void AddCommand(ldfschedulecommand *command);
	void ValidateUnicity(ldfscheduletable *table, uint8_t **validation_messages, uint32_t *validation_messages_count);
	void ValidateFrames(ldfframe **frames, uint32_t frames_count, uint8_t **validation_messages, uint32_t *validation_messages_count);

};

}

#endif /* LIN_LDFSCHEDULETABLE_H_ */
