/*
 * ldfscheduletable.cpp
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <stdio.h>
#include <ldfcommon.h>
#include <ldfscheduletable.h>


namespace lin
{

ldfscheduletable::ldfscheduletable(uint8_t *name)
{
	this->name = (uint8_t *)strdup((char *) name);
	this->commands_number = 0;
}

ldfscheduletable::~ldfscheduletable()
{
	if (name) delete name;
	while (commands_number > 0) delete commands[--commands_number];
}

void ldfscheduletable::AddCommand(ldfschedulecommand *command)
{
	commands[commands_number++] = command;
}

void ldfscheduletable::ValidateUnicity(ldfscheduletable *table, uint8_t **validation_messages, uint32_t *validation_messages_count)
{
	char str[1000];

	if (strcmp((char *)name, (char *)table->name) == 0)
	{
		sprintf(str, STR_ERR "Schedule table name '%s' repeated.", name);
		validation_messages[*validation_messages_count++] = (uint8_t *)strdup(str);
	}
}

void ldfscheduletable::ValidateFrames(ldfframe **frames, uint32_t frames_count, uint8_t **validation_messages, uint32_t *validation_messages_count)
{
	char str[1000];
	uint32_t i, j;

	for (i = 0; i < commands_number; i++)
	{
		ldfframe *f = NULL;

		// Look for frame definition
		for (j = 0; (f == NULL) && (j < frames_count); j++)
		{
			f = (strcmp((char *)frames[j]->GetName(), (char *)commands[i]->GetFrameName()) == 0) ? frames[j] : NULL;
		}

		// Check frame exists
		if (f == NULL)
		{
			sprintf(str, STR_ERR "Schedule table '%s' command frame '%s' not defined.", name, commands[i]->GetFrameName());
			validation_messages[*validation_messages_count++] = (uint8_t *)strdup(str);
			continue;
		}

		// Check configurable frame repeated
		for (j = i + 1; j < commands_number; j++)
		{
			commands[i]->ValidateUnicity(name, commands[j], validation_messages, validation_messages_count);
		}
	}
}

}
