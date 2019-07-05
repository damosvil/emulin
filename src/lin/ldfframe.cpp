/*
 * ldfframe.cpp
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <stdlib.h>
#include <ldfframe.h>

#define BLANK_CHARACTERS					" \t\r\n"


namespace lin {

ldfframe::ldfframe(uint8_t *name, uint8_t id, uint8_t *publisher, uint8_t size)
{
	this->name = (uint8_t *)strdup((char *) name);
	this->id = id;
	this->publisher = (uint8_t *)strdup((char *)publisher);
	this->size = size;
	this->signals_count = 0;
}

ldfframe::~ldfframe()
{
	if (name) delete name;
	if (publisher) delete publisher;
	while (signals_count--) delete signals[signals_count];
}

ldfframe *ldfframe::FromLdfStatement(uint8_t *statement)
{
	char *p = NULL;
	char *name = NULL;
	char *publisher = NULL;
	uint8_t id = 0xFF;
	uint8_t frame_size = 0;

	// Name
	p = strtok((char *)statement, ":," BLANK_CHARACTERS);
	if (p) name = p;

	// ID
	if (p) p = strtok(NULL, ":," BLANK_CHARACTERS);
	if (p) id = (p[1] == 'x' || p[1] == 'X') ? strtol(p, NULL, 16) : atoi(p);

	// Publisher
	if (p) p = strtok(NULL, ":," BLANK_CHARACTERS);
	if (p) publisher = p;

	// Frame size
	if (p) p = strtok(NULL, ":," BLANK_CHARACTERS);
	if (p) frame_size = atoi(p);

	// Validate and return a new frame
	if (name != NULL && publisher != NULL && id != 0xFF && frame_size != 0)
	{
		return new ldfframe((uint8_t *)name, id, (uint8_t *)publisher, frame_size);
	}
	else
	{
		return NULL;
	}
}

void ldfframe::AddSignal(ldfframesignal *signal)
{
	signals[signals_count++] = signal;
}


} /* namespace lin */
