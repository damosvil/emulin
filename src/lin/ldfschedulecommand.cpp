/*
 * ldfschedulecommand.cpp
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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
	if (p == NULL) return NULL;
	frame_name = p;

	// Check delay word
	p = strtok(NULL, BLANK_CHARACTERS);
	if (p == NULL) return NULL;
	if (strcmp(p, "delay") != 0) return NULL;

	// Timeout
	p = strtok(NULL, BLANK_CHARACTERS);
	if (p == NULL) return NULL;
	timeout = atof(p);

	// Check ms word
	p = strtok(NULL, BLANK_CHARACTERS);
	if (p == NULL) return NULL;
	if (strcmp(p, "ms") != 0) return NULL;

	// Return command
	return new ldfschedulecommand((uint8_t *)frame_name, timeout);
}

uint8_t *ldfschedulecommand::GetFrameName()
{
	return frame_name;
}

uint16_t ldfschedulecommand::GetTimeoutMs()
{
	return timeout;
}

void ldfschedulecommand::ValidateUnicity(uint8_t *schedule_table, ldfschedulecommand *command, uint8_t **validation_messages, uint32_t *validation_messages_count)
{
	char str[1000];

	if (strcmp((char *)frame_name, (char *)command->frame_name) == 0)
	{
		sprintf(str, STR_ERR "Schedule table '%s' schedule command frame name '%s' repeated.", schedule_table, frame_name);
		validation_messages[*validation_messages_count++] = (uint8_t *)strdup(str);
	}
}


} /* namespace lin */
