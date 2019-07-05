/*
 * ldfscheduletable.h
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFSCHEDULETABLE_H_
#define LIN_LDFSCHEDULETABLE_H_

#include <stdint.h>
#include <ldfschedulecommand.h>


namespace lin
{

class ldfscheduletable {

private:
	uint8_t *name;

	// Schedule commands
	ldfschedulecommand *commands[1000];
	uint16_t commands_number;

public:
	ldfscheduletable(uint8_t *name);
	virtual ~ldfscheduletable();

	void AddCommand(ldfschedulecommand *command);

};

}

#endif /* LIN_LDFSCHEDULETABLE_H_ */
