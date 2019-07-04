/*
 * ldfmasternode.cpp
 *
 *  Created on: 3 jul. 2019
 *      Author: ctag
 */

#include <ldfmasternode.h>

namespace lin {

ldfmasternode::ldfmasternode(uint8_t *name, uint16_t timebase, uint16_t jitter) : ldfnode(name)
{
	this->timebase = timebase;
	this->jitter = jitter;
}

ldfmasternode::~ldfmasternode() {
}

}
