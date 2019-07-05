/*
 * ldfconfigurableframe.h
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFCONFIGURABLEFRAME_H_
#define LIN_LDFCONFIGURABLEFRAME_H_

#include <stdint.h>

namespace lin {

class ldfconfigurableframe {

private:
	uint8_t *name;
	uint8_t id;

public:
	ldfconfigurableframe(uint8_t *name, uint8_t id);
	virtual ~ldfconfigurableframe();

	static ldfconfigurableframe *FromUdsStatement(uint8_t *statement);

};

} /* namespace lin */

#endif /* LIN_LDFCONFIGURABLEFRAME_H_ */
