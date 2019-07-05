/*
 * ldfframesignal.cpp
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <stdlib.h>
#include <ldfcommon.h>
#include <ldfframesignal.h>


namespace lin {

ldfframesignal::ldfframesignal(uint8_t *name, uint16_t offset)
{
	this->name = (uint8_t *)strdup((char *)name);
	this->offset = offset;
}

ldfframesignal::~ldfframesignal()
{
	if (name) delete name;
}

ldfframesignal *ldfframesignal::FromLdfStatement(uint8_t *statement)
{
	char *p = NULL;
	char *name = NULL;
	uint16_t offset = 0;

	// Name
	p = strtok((char *)statement, ":," BLANK_CHARACTERS);
	if (p) name = p;

	// ID
	if (p) p = strtok(NULL, ":," BLANK_CHARACTERS);
	if (p) offset = atoi(p);

	if (name)
	{
		return new ldfframesignal((uint8_t *)name, offset);
	}
	else
	{
		return NULL;
	}

}


} /* namespace lin */
