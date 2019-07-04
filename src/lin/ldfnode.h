/*
 * ldfnode.h
 *
 *  Created on: 3 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFNODE_H_
#define LIN_LDFNODE_H_

#include <stdint.h>

namespace lin {

class ldfnode {

private:
	uint8_t *name;

public:
	ldfnode(uint8_t *name);
	virtual ~ldfnode();

};

}

#endif /* LIN_LDFNODE_H_ */
