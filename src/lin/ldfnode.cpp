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

bool ldfnode::CheckNodeName(uint8_t *name, ldfnode *master, ldfnode **slaves, uint32_t slaves_count)
{
	bool name_ok = false;
	uint32_t i;

	// Check publisher is master
	if (master != NULL)
	{
		name_ok = strcmp((char *)name, (char *)master->GetName()) == 0;
	}

	// Check publisher in slaves
	for (i = 0; !name_ok && (i < slaves_count); i++)
	{
		name_ok = strcmp((char *)name, (char *)slaves[i++]->GetName()) == 0;
	}

	return name_ok;
}

uint8_t *ldfnode::GetName()
{
	return name;
}

void ldfnode::SetName(uint8_t *name)
{
	if (this->name) delete name;
	this->name = (name != NULL) ? (uint8_t *)strdup((char *)name) : NULL;
}

}
