/*
 * ldfnode.cpp
 *
 *  Created on: 3 jul. 2019
 *      Author: iso9660
 */

#include <ldfnode.h>
#include <string.h>

namespace lin {

ldfnode::ldfnode(uint8_t *name) {
	// Store the name length
	uint32_t length = name ? strlen((char *)name) : 0;

	// Make room to copy the name
	this->name = name ? new uint8_t[length + 1] : NULL;

	// Copy the name
	if (length)
	{
		strcpy((char *)this->name, (char *)name);
	}
}

ldfnode::~ldfnode() {
	// Remove the name
	if (name) delete name;
}

}
