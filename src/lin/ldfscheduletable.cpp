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

ldfscheduletable::ldfscheduletable(const uint8_t *name)
{
	this->name = StrDup(name);
	this->commands_count = 0;
}

ldfscheduletable::~ldfscheduletable()
{
	if (name) delete name;
	while (commands_count > 0) delete commands[--commands_count];
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
	return commands_count;
}

void ldfscheduletable::UpdateCommandsFrameName(const uint8_t *old_name, const uint8_t *new_name)
{
	for (int i = 0; i < commands_count; i++)
	{
		commands[i]->UpdateFrameName(old_name, new_name);
	}
}

void ldfscheduletable::UpdateCommandsSlaveName(const uint8_t *old_name, const uint8_t *new_name)
{
	for (int i = 0; i < commands_count; i++)
	{
		commands[i]->UpdateSlaveName(old_name, new_name);
	}
}

void ldfscheduletable::DeleteCommandsByFrameName(const uint8_t *name)
{
	for (int i = 0; i < commands_count; i++)
		if (StrEq(name, commands[i]->GetFrameName()))
			DeleteCommandByIndex(i);
}

void ldfscheduletable::DeleteCommandsBySlaveName(const uint8_t *name)
{
	for (int i = 0; i < commands_count; i++)
		if (StrEq(name, commands[i]->GetSlaveName()))
			DeleteCommandByIndex(i);
}

void ldfscheduletable::DeleteCommandByIndex(uint32_t ix)
{
	delete commands[ix];
	commands_count--;
	for (; ix < commands_count; ix++)
		commands[ix] = commands[ix + 1];
}

void ldfscheduletable::AddCommand(ldfschedulecommand *command)
{
	commands[commands_count++] = command;
}

void ldfscheduletable::ValidateUnicity(ldfscheduletable *table, uint8_t **validation_messages, uint32_t *validation_messages_count)
{
	char str[1000];

	if (StrEq(name, table->name))
	{
		sprintf(str, STR_ERR "Schedule table name '%s' repeated.", name);
		validation_messages[*validation_messages_count++] = StrDup(str);
	}
}

void ldfscheduletable::ValidateFrames(ldfframe **frames, uint32_t frames_count, uint8_t **validation_messages, uint32_t *validation_messages_count)
{
	char str[1000];
	uint32_t i, j;

	for (i = 0; i < commands_count; i++)
	{
		ldfframe *f = NULL;

		// Look for frame definition
		for (j = 0; (f == NULL) && (j < frames_count); j++)
		{
			f = StrEq(frames[j]->GetName(), commands[i]->GetFrameName()) ? frames[j] : NULL;
		}

		// Check frame exists
		if (f == NULL)
		{
			sprintf(str, STR_ERR "Schedule table '%s' command frame '%s' not defined.", name, commands[i]->GetFrameName());
			validation_messages[*validation_messages_count++] = StrDup(str);
			continue;
		}

		// Check configurable frame repeated
		for (j = i + 1; j < commands_count; j++)
		{
			commands[i]->ValidateUnicity(name, commands[j], validation_messages, validation_messages_count);
		}
	}
}

void ldfscheduletable::ToLdfFile(FILE *f, ldf *db)
{
	fprintf(f, "    %s {\r\n", name);
	for (uint32_t i = 0; i < commands_count; i++)
		fprintf(f, "        %s delay %d ms;\r\n", commands[i]->GetStrCommand(db), commands[i]->GetTimeoutMs());
	fprintf(f, "    }\r\n");
}

}
