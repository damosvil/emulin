/*
 * ldfnode.cpp
 *
 *  Created on: 3 jul. 2019
 *      Author: iso9660
 */

#include <ldfnode.h>
#include <string.h>
#include <ldfcommon.h>

namespace lin {

ldfnode::ldfnode(uint8_t *name) {
	// Copy the name
	this->name = name != NULL ? StrDup(name) : NULL;
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
		name_ok = StrEq(name, master->name);
	}

	// Check publisher in slaves
	for (i = 0; !name_ok && (i < slaves_count); i++)
	{
		name_ok = StrEq(name, slaves[i]->name);
	}

	return name_ok;
}

uint8_t *ldfnode::GetName()
{
	return name;
}

void ldfnode::UpdateName(const uint8_t *old_name, const uint8_t *new_name)
{
	if (!StrEq(old_name, name))
		return;

	delete name;
	name = StrDup(new_name);
}


}
