/*
 * ldfframesignal.h
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFFRAMESIGNAL_H_
#define LIN_LDFFRAMESIGNAL_H_

#include <stdint.h>

namespace lin {

class ldfframesignal {

private:
	uint8_t *name;
	uint16_t offset;

public:
	ldfframesignal(uint8_t *name, uint16_t offset);
	virtual ~ldfframesignal();

	static ldfframesignal *FromLdfStatement(uint8_t *statement);

	uint8_t *GetName();
	uint16_t GetOffset();
	void SetName(const uint8_t *name);
	void SetOffset(uint16_t offset);

	bool NameIs(const uint8_t *name);

};

} /* namespace lin */

#endif /* LIN_LDFFRAMESIGNAL_H_ */
