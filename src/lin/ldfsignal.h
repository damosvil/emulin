/*
 * ldfsignal.h
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFSIGNAL_H_
#define LIN_LDFSIGNAL_H_

#include <stdint.h>

namespace lin {

class ldfsignal {

private:
	uint8_t *name;
	uint8_t bit_size;
	uint32_t default_value;
	uint8_t *publisher;
	uint8_t *subscribers[128];
	uint8_t subscribers_count;

public:
	ldfsignal(uint8_t *name, uint8_t bit_size, uint32_t default_value, uint8_t *publisher, uint8_t **subscribers, uint8_t subscribers_count);
	virtual ~ldfsignal();

	static ldfsignal *FromLdfStatement(uint8_t *statement);

};

} /* namespace lin */

#endif /* LIN_LDFSIGNAL_H_ */
