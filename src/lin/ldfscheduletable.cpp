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

uint8_t *ldfscheduletable::GetName()
{
	return name;
}

ldfschedulecommand *ldfscheduletable::GetCommandByIndex(uint32_t ix)
{
	return commands[ix];
}

uint16_t ldfscheduletable::GetCommandsCount()
{
	return commands_number;
}

void ldfscheduletable::UpdateCommandName(const uint8_t *old_name, const uint8_t *new_name)
{
	for (int i = 0; i < commands_number; i++)
	{
		commands[i]->UpdateName(old_name, new_name);
	}
}

void ldfscheduletable::DeleteCommand(const uint8_t *name)
{
	for (int i = 0; i < commands_number; )
	{
		// Skip commands with different name
		if (strcmp((char *)name, (char*)commands[i]->GetName()) != 0)
		{
			i++;
			continue;
		}

		// Delete command and move back the rest ones
		delete commands[i];
		commands_number--;
		for (int j = i; j < commands_number; j++)
			commands[j] = commands[j + 1];
	}
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
			f = (strcmp((char *)frames[j]->GetName(), (char *)commands[i]->GetName()) == 0) ? frames[j] : NULL;
		}

		// Check frame exists
		if (f == NULL)
		{
			sprintf(str, STR_ERR "Schedule table '%s' command frame '%s' not defined.", name, commands[i]->GetName());
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
