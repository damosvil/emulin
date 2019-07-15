/*
 * ldfsignalencoding.h
 *
 *  Created on: 15 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFSIGNALENCODING_H_
#define LIN_LDFSIGNALENCODING_H_

#include <ldflogicalvalue.h>
#include <ldfphysicalvalue.h>


namespace lin {

class ldfsignalencoding {

private:
	uint8_t *name;
	bool treat_as_bcd;
	bool treat_as_ascii;
	ldfphysicalvalue *physical_value;
	ldflogicalvalue *logical_values[10000];
	uint32_t logical_values_count;


public:
	ldfsignalencoding(uint8_t *name);
	virtual ~ldfsignalencoding();

	void UpdateFromLdfStatement(uint8_t *statement);

};

} /* namespace lin */

#endif /* LIN_LDFSIGNALENCODING_H_ */
