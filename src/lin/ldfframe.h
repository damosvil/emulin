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
	uint8_t *GetName();
	uint8_t GetId();
	uint8_t *GetPublisher();
	uint8_t GetSize();
	ldfframesignal *GetSignal(uint32_t ix);
	uint32_t GetSignalsCount();

	void ValidatePublisher(ldfnode *master, ldfnode **slaves, uint32_t slaves_count, uint8_t **validation_messages, uint32_t *validation_messages_count);
	void ValidateUnicity(ldfframe *frame, uint8_t **validation_messages, uint32_t *validation_messages_count);
	void ValidateSignals(ldfsignal **signals, uint32_t signals_count, uint8_t **validation_messages, uint32_t *validation_messages_count);

	static int32_t CompareId(const ldfframe *a, const ldfframe *b);
	static int32_t ComparePublisher(const ldfframe *a, const ldfframe *b);
};

} /* namespace lin */

#endif /* LIN_LDFFRAME_H_ */
