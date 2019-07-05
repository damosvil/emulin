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

};

} /* namespace lin */

#endif /* LIN_LDFSCHEDULECOMMAND_H_ */
