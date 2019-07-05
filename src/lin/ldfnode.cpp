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
	// Copy the name
	this->name = name != NULL ? (uint8_t *)strdup((char *)name) : NULL;
}

ldfnode::~ldfnode() {
	// Remove the name
	if (name) delete name;
}

}
