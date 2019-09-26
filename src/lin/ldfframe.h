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
	ldfframe(const uint8_t *name, uint8_t id, const uint8_t *publisher, uint8_t size);
	virtual ~ldfframe();

	static ldfframe *FromLdfStatement(uint8_t *statement);

	uint8_t *GetName();
	uint8_t GetId();
	uint8_t GetPid();
	uint8_t *GetPublisher();
	uint8_t GetSize();
	ldfframesignal *GetSignal(uint32_t ix);
	uint32_t GetSignalsCount();

	void AddSignal(ldfframesignal *signal);
	void DeleteSignalByIndex(uint32_t ix);
	void DeleteSignalByName(const uint8_t *signal_name);
	void UpdateSignalName(const uint8_t *old_signal_name, const uint8_t *new_signal_name);
	void UpdateNodeName(const uint8_t *old_name, const uint8_t *new_name);

	void ValidatePublisher(ldfnode *master, ldfnode **slaves, uint32_t slaves_count, uint8_t **validation_messages, uint32_t *validation_messages_count);
	void ValidateUnicity(ldfframe *frame, uint8_t **validation_messages, uint32_t *validation_messages_count);
	void ValidateSignals(ldfsignal **signals, uint32_t signals_count, uint8_t **validation_messages, uint32_t *validation_messages_count);

	static int32_t CompareId(const ldfframe *a, const ldfframe *b);
	static int32_t ComparePublisher(const ldfframe *a, const ldfframe *b);
	void SortData();

};

} /* namespace lin */

#endif /* LIN_LDFFRAME_H_ */
