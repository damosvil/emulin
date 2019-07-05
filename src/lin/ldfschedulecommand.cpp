/*
 * ldfschedulecommand.cpp
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <stdlib.h>
#include <ldfcommon.h>
#include <ldfschedulecommand.h>

namespace lin {

ldfschedulecommand::ldfschedulecommand(uint8_t *frame_name, uint16_t timeout)
{
	this->frame_name = (uint8_t *)strdup((char *) frame_name);
	this->timeout = timeout;
}

ldfschedulecommand::~ldfschedulecommand()
{
	if (frame_name) delete frame_name;
}

ldfschedulecommand *ldfschedulecommand::FromLdfStatement(uint8_t *statement)
{
	char *p;
	char *frame_name = NULL;
	uint16_t timeout = 0;

	// Frame name
	p = strtok((char *) statement, BLANK_CHARACTERS);
	if (p) frame_name = p;

	// Skip delay word, and timeout
	if (p) p = strtok(NULL, BLANK_CHARACTERS);
	if (p) p = strtok(NULL, BLANK_CHARACTERS);
	if (p) timeout = atoi(p);

	// Validate and return the command
	if (frame_name)
	{
		return new ldfschedulecommand((uint8_t *)frame_name, timeout);
	}
	else
	{
		return NULL;
	}
}


} /* namespace lin */
