/*
 * ldfsignal.h
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFSIGNAL_H_
#define LIN_LDFSIGNAL_H_

#include <stdint.h>
#include <ldfnode.h>

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

	void ValidateNodes(ldfnode *master, ldfnode **slaves, uint32_t slaves_count, uint8_t **validation_messages, uint32_t *validation_messages_count);
	uint8_t *GetName();

};

} /* namespace lin */

#endif /* LIN_LDFSIGNAL_H_ */
