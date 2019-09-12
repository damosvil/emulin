/*
 * ldfmasternode.h
 *
 *  Created on: 3 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFMASTERNODE_H_
#define LIN_LDFMASTERNODE_H_

#include <stdint.h>
#include <ldfnode.h>

namespace lin {

class ldfmasternode : public lin::ldfnode {

private:
	uint16_t timebase;
	uint16_t jitter;

public:
	ldfmasternode(const uint8_t *name, uint16_t timebase, uint16_t jitter);
	virtual ~ldfmasternode();

	static ldfmasternode *FromLdfStatement(const uint8_t *statement);

	uint16_t GetTimebase();
	void SetTimebase(uint16_t timebase);

	uint16_t GetJitter();
	void SetJitter(uint16_t jitter);

};

}

#endif /* LIN_LDFMASTERNODE_H_ */
