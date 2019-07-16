/*
 * ldfencodingsignals.h
 *
 *  Created on: 16 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFENCODINGSIGNALS_H_
#define LIN_LDFENCODINGSIGNALS_H_

#include <stdint.h>


namespace lin {

class ldfencodingsignals {

private:
	uint8_t *encoding_name;
	uint8_t *signals[1000];
	uint32_t signals_count;

public:
	ldfencodingsignals(uint8_t *encoding_name);
	virtual ~ldfencodingsignals();

	void AddSignal(uint8_t *signal);

	static ldfencodingsignals *FromLdfStatement(uint8_t *statement);

};

} /* namespace lin */

#endif /* LIN_LDFENCODINGSIGNALS_H_ */
