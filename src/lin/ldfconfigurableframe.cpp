/*
 * ldfconfigurableframe.cpp
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ldfcommon.h>
#include <ldfconfigurableframe.h>

namespace lin {

ldfconfigurableframe::ldfconfigurableframe(uint8_t *name, uint8_t id)
{
	this->name = StrDup(name);
	this->id = id;
}

ldfconfigurableframe::~ldfconfigurableframe()
{
	if (name) delete name;
}

ldfconfigurableframe *ldfconfigurableframe::FromLdfStatement(uint8_t *statement)
{
	char *p;
	char *name = NULL;
	uint8_t id = 0xFF;

	p = strtok((char *) statement, "=" BLANK_CHARACTERS);
	if (p) name = p;

	if (p) p = strtok(NULL, "=" BLANK_CHARACTERS);
	if (p) id = ParseInt(p);

	if (name)
	{
		return new ldfconfigurableframe((uint8_t *)name, id);
	}
	else
	{
		return NULL;
	}
}

int32_t ldfconfigurableframe::SorterConfigurableFrames(const void *a, const void *b)
{
	ldfconfigurableframe *aa = *(ldfconfigurableframe **)a;
	ldfconfigurableframe *bb = *(ldfconfigurableframe **)b;

	int res = aa->id - bb->id;
	if (res == 0)
	{
		res = strcmp((char *)aa->name, (char *)bb->name);
	}
	return res;
}

uint8_t *ldfconfigurableframe::GetName()
{
	return name;
}

uint8_t ldfconfigurableframe::GetId()
{
	return id;
}

void ldfconfigurableframe::ValidateUnicity(uint8_t *attributes, ldfconfigurableframe *frame, uint8_t **validation_messages, uint32_t *validation_messages_count)
{
	char str[1000];

	if (StrEq((char *)name, (char *)frame->name))
	{
		sprintf(str, STR_ERR "Node_attributes '%s' configurable frame name '%s' repeated.", attributes, name);
		validation_messages[*validation_messages_count++] = StrDup(str);
	}

	if (id == frame->id)
	{
		sprintf(str, STR_ERR "Node_attributes '%s' configurable frame ID 0x%X repeated.", attributes, id);
		validation_messages[*validation_messages_count++] = StrDup(str);
	}
}

void ldfconfigurableframe::UpdateName(const uint8_t *old_frame_name, const uint8_t *new_frame_name)
{
	if (StrEq((char *)name, (char *)old_frame_name))
	{
		delete name;
		name = StrDup(new_frame_name);
	}
}


} /* namespace lin */
