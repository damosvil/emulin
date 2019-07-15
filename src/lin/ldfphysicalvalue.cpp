/*
 * ldfphysicalvalue.cpp
 *
 *  Created on: 15 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <stdlib.h>
#include <ldfcommon.h>
#include <ldfphysicalvalue.h>

namespace lin {

ldfphysicalvalue::ldfphysicalvalue(float min, float max, float scale, float offset, uint8_t *description)
{
	this->min = min;
	this->max = max;
	this->scale = scale;
	this->offset = offset;
	this->description = (uint8_t *)strdup((char *)description);

}

ldfphysicalvalue::~ldfphysicalvalue()
{
	if (description) delete description;
}

ldfphysicalvalue *ldfphysicalvalue::FromLdfStatement(uint8_t *statement)
{
	float min;
	float max;
	float scale;
	float offset;

	// Min
	char *p = strtok((char *) statement, "," BLANK_CHARACTERS);
	if (!p) return NULL;
	min = strtof(p, NULL);

	// Max
	p = strtok(NULL, "," BLANK_CHARACTERS);
	if (!p) return NULL;
	max = strtof(p, NULL);

	// Scale
	p = strtok(NULL, "," BLANK_CHARACTERS);
	if (!p) return NULL;
	scale = strtof(p, NULL);

	// Offset
	p = strtok(NULL, "," BLANK_CHARACTERS);
	if (!p) return NULL;
	offset = strtof(p, NULL);

	// Description
	p = strtok(NULL, "\"");

	// Return a new physical value
	return new ldfphysicalvalue(min, max, scale, offset, (uint8_t *) p);
}

} /* namespace lin */
