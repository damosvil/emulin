/*
 * ldflogicalvalue.h
 *
 *  Created on: 15 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFLOGICALVALUE_H_
#define LIN_LDFLOGICALVALUE_H_

#include <stdint.h>


namespace lin {

class ldflogicalvalue {

private:
	uint32_t value;
	uint8_t *description;

public:
	ldflogicalvalue(uint32_t value, uint8_t *description);
	virtual ~ldflogicalvalue();

	static ldflogicalvalue *FromLdfStatement(uint8_t *statement);

};

} /* namespace lin */

#endif /* LIN_LDFLOGICALVALUE_H_ */
