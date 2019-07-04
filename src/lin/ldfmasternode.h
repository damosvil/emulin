/*
 * ldfmasternode.h
 *
 *  Created on: 3 jul. 2019
 *      Author: ctag
 */

#ifndef LIN_LDFMASTERNODE_H_
#define LIN_LDFMASTERNODE_H_

#include <stdint.h>
#include <ldfnode.h>

namespace lin {

class ldfmasternode : private lin::ldfnode {

private:
	uint16_t timebase;
	uint16_t jitter;

public:
	ldfmasternode(uint8_t *name, uint16_t timebase, uint16_t jitter);
	virtual ~ldfmasternode();

};

}

#endif /* LIN_LDFMASTERNODE_H_ */
