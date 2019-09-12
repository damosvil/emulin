/*
 * ldfphysicalvalue.h
 *
 *  Created on: 15 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFPHYSICALVALUE_H_
#define LIN_LDFPHYSICALVALUE_H_

#include <stdint.h>

namespace lin {

class ldfphysicalvalue {

private:
	float min;
	float max;
	float scale;
	float offset;
	uint8_t *description;

public:
	ldfphysicalvalue(float min, float max, float scale, float offset, const uint8_t *description);
	virtual ~ldfphysicalvalue();

	static ldfphysicalvalue *FromLdfStatement(const uint8_t *statement);

};

} /* namespace lin */

#endif /* LIN_LDFPHYSICALVALUE_H_ */
