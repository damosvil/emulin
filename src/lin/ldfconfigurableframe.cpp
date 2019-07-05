/*
 * ldfconfigurableframe.cpp
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <stdlib.h>
#include <ldfcommon.h>
#include <ldfconfigurableframe.h>

namespace lin {

ldfconfigurableframe::ldfconfigurableframe(uint8_t *name, uint8_t id)
{
	this->name = (uint8_t *)strdup((char *)name);
	this->id = id;
}

ldfconfigurableframe::~ldfconfigurableframe()
{
	if (name) delete name;
}

ldfconfigurableframe *ldfconfigurableframe::FromUdsStatement(uint8_t *statement)
{
	char *p;
	char *name = NULL;
	uint8_t id = 0xFF;

	p = strtok((char *) statement, "=" BLANK_CHARACTERS);
	if (p) name = p;

	if (p) p = strtok(NULL, "=" BLANK_CHARACTERS);
	if (p) id = (p[1] == 'x' || p[1] == 'X') ? strtol(p, NULL, 16) : atoi(p);

	if (name)
	{
		return new ldfconfigurableframe((uint8_t *)name, id);
	}
	else
	{
		return NULL;
	}
}

} /* namespace lin */
