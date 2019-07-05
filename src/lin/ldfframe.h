/*
 * ldfframe.h
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFFRAME_H_
#define LIN_LDFFRAME_H_

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

};

} /* namespace lin */

#endif /* LIN_LDFFRAME_H_ */
