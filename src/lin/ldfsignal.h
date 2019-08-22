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
	void ValidateUnicity(ldfsignal *signal, uint8_t **validation_messages, uint32_t *validation_messages_count);
	const uint8_t *GetName();
	uint8_t GetBitSize();
	uint32_t GetDefaultValue();
	const uint8_t *GetPublisher();
	uint32_t GetSubscribersCount();
	uint8_t *GetSubscriber(uint32_t ix);

	bool NameIs(uint8_t *name);
	bool UsesSlave(uint8_t *slave_name);
	void UpdateNodeName(uint8_t *old_name, uint8_t *new_name);

	static int32_t Compare(const ldfsignal *a, const ldfsignal *b);
	static int32_t ComparePublisher(const ldfsignal *a, const ldfsignal *b);

};

} /* namespace lin */

#endif /* LIN_LDFSIGNAL_H_ */
