/*
 * ldflogicalvalue.cpp
 *
 *  Created on: 15 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <stdlib.h>
#include <ldfcommon.h>
#include <ldflogicalvalue.h>

namespace lin {

ldflogicalvalue::ldflogicalvalue(uint32_t value, const uint8_t *description)
{
	this->value = value;
	this->description = StrDup(description);
}

ldflogicalvalue::~ldflogicalvalue()
{
	if (description) delete description;
}


ldflogicalvalue *ldflogicalvalue::FromLdfStatement(const uint8_t *statement)
{
	uint32_t value;

	// Read value
	char *p = strtok((char *)statement, "," BLANK_CHARACTERS);
	if (!p) return NULL;
	value = ParseInt(p);

	// Read description
	p = strtok(NULL, "\"");

	// Return a new logical value
	return new ldflogicalvalue(value, Str(p));
}


} /* namespace lin */
