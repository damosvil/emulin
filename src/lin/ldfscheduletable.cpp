/*
 * ldfscheduletable.cpp
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
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

}
