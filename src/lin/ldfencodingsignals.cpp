/*
 * ldfencodingsignals.cpp
 *
 *  Created on: 16 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <stdlib.h>
#include <ldfcommon.h>
#include <ldfencodingsignals.h>


namespace lin {

ldfencodingsignals::ldfencodingsignals(uint8_t *encoding_name)
{
	this->encoding_name = (uint8_t *)strdup((char *)encoding_name);
	this->signals_count = 0;
}

ldfencodingsignals::~ldfencodingsignals()
{
	if (encoding_name) delete encoding_name;
	while (signals_count > 0) delete signals[--signals_count];
}

void ldfencodingsignals::AddSignal(uint8_t *signal)
{
	signals[signals_count++] = (uint8_t *)strdup((char *)signal);
}

ldfencodingsignals *ldfencodingsignals::FromLdfStatement(uint8_t *statement)
{
	char *encoding_name;
	ldfencodingsignals *s;

	// Type name
	char *p = strtok((char *)statement, ":," BLANK_CHARACTERS);
	if (!p) return NULL;
	encoding_name = p;

	// First signal
	p = strtok(NULL, "," BLANK_CHARACTERS);
	if (!p) return NULL;

	// Create relationship between encoding and signal
	s = new ldfencodingsignals((uint8_t *)encoding_name);

	// If any add more signals
	while (p)
	{
		s->AddSignal((uint8_t *) p);
		p = strtok(NULL, "," BLANK_CHARACTERS);
	}

	return s;
}



} /* namespace lin */
