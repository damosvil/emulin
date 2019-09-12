/*
 * ldfsignalencoding.h
 *
 *  Created on: 15 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFENCODINGTYPE_H_
#define LIN_LDFENCODINGTYPE_H_

#include <ldflogicalvalue.h>
#include <ldfphysicalvalue.h>


namespace lin {

class ldfencodingtype {

private:
	uint8_t *name;
	bool treat_as_bcd;
	bool treat_as_ascii;
	ldfphysicalvalue *physical_value;
	ldflogicalvalue *logical_values[10000];
	uint32_t logical_values_count;


public:
	ldfencodingtype(const uint8_t *name);
	virtual ~ldfencodingtype();

	void UpdateFromLdfStatement(uint8_t *statement);

};

} /* namespace lin */

#endif /* LIN_LDFENCODINGTYPE_H_ */
