/*
 * ldfframe.h
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFFRAME_H_
#define LIN_LDFFRAME_H_

#include <ldfnode.h>
#include <ldfsignal.h>
#include <ldfframesignal.h>


namespace lin {


class ldfframe {

private:
	uint8_t *name;
	uint8_t id;
	uint8_t *publisher;
	uint8_t size;

	ldfframesignal *signals[1000];
	uint8_t signals_count;

public:
	ldfframe(uint8_t *name, uint8_t id, uint8_t *publisher, uint8_t size);
	virtual ~ldfframe();

	static ldfframe *FromLdfStatement(uint8_t *statement);

	void AddSignal(ldfframesignal *signal);
	void ValidatePublisher(ldfnode *master, ldfnode **slaves, uint32_t slaves_count, uint8_t **validation_messages, uint32_t *validation_messages_count);
	void ValidateUnicity(ldfframe *frame, uint8_t **validation_messages, uint32_t *validation_messages_count);
	void Validate(ldfsignal **signals, uint32_t signals_count, uint8_t **validation_messages, uint32_t *validation_messages_count);

};

} /* namespace lin */

#endif /* LIN_LDFFRAME_H_ */