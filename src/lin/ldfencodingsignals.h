/*
 * ldfencodingsignals.h
 *
 *  Created on: 16 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFENCODINGSIGNALS_H_
#define LIN_LDFENCODINGSIGNALS_H_

#include <stdint.h>
#include <ldfsignal.h>


namespace lin {

class ldfencodingsignals {

private:
	uint8_t *encoding_name;
	uint8_t *signals[1000];
	uint32_t signals_count;

public:
	ldfencodingsignals(const uint8_t *encoding_name);
	virtual ~ldfencodingsignals();

	void AddSignal(const uint8_t *signal);

	static ldfencodingsignals *FromLdfStatement(const uint8_t *statement);

	void ValidateUnicity(ldfencodingsignals *encoding, uint8_t **validation_messages, uint32_t *validation_messages_count);
	void ValidateSignals(ldfsignal **signals, uint32_t signals_count, uint8_t **validation_messages, uint32_t *validation_messages_count);

};

} /* namespace lin */

#endif /* LIN_LDFENCODINGSIGNALS_H_ */
