/*
 * ldfsignal.cpp
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ldfcommon.h>
#include <ldfsignal.h>
#include <ldfnode.h>


namespace lin {

ldfsignal::ldfsignal(uint8_t *name, uint8_t bit_size, uint32_t default_value, uint8_t *publisher, uint8_t **subscribers, uint8_t subscribers_count)
{
	// Copy name
	this->name = (uint8_t *)strdup((char *)name);
	this->bit_size = bit_size;
	this->default_value = default_value;
	this->publisher = (uint8_t *)strdup((char *) publisher);
	for (this->subscribers_count = 0; this->subscribers_count < subscribers_count; this->subscribers_count++)
		this->subscribers[this->subscribers_count] = (uint8_t *)strdup((char *)subscribers[this->subscribers_count]);
}

ldfsignal::~ldfsignal()
{
	// Erase resources
	if (name != NULL)
		delete name;
	if (publisher != NULL)
		delete publisher;
	while (subscribers_count > 0)
		delete subscribers[--subscribers_count];
}

ldfsignal *ldfsignal::FromLdfStatement(uint8_t *statement)
{
	char *p = NULL;
	char *name = NULL;
	uint8_t bit_size = 0;
	uint32_t default_value = 0;
	char *publisher = NULL;
	char *subscribers[128];
	uint8_t subscribers_count = 0;

	// Signal name
	p = strtok((char *)statement, ":," BLANK_CHARACTERS);
	if (!p) return NULL;
	name = p;

	// Bit size
	p = strtok(NULL, ":," BLANK_CHARACTERS);
	if (!p) return NULL;
	bit_size = atoi(p);

	// Default value
	p = strtok(NULL, ":," BLANK_CHARACTERS);
	if (!p) return NULL;
	default_value = (p[1] == 'x' || p[1] == 'X') ? strtol(p, NULL, 16) : atoi(p);

	// Publisher
	p = strtok(NULL, ":," BLANK_CHARACTERS);
	if (!p) return NULL;
	publisher = p;

	// Subscribers
	while (p)
	{
		p = strtok(NULL, ":," BLANK_CHARACTERS);
		if (p) subscribers[subscribers_count++] = p;
	}

	// Validate and return new signal
	if (name != NULL && publisher != NULL && subscribers_count != 0)
	{
		return new ldfsignal(
				(uint8_t *)name, bit_size, default_value,
				(uint8_t *)publisher,
				(uint8_t **)subscribers, subscribers_count);
	}
	else
	{
		return NULL;
	}
}

void ldfsignal::ValidateNodes(ldfnode *master, ldfnode **slaves, uint32_t slaves_count, uint8_t **validation_messages, uint32_t *validation_messages_count)
{
	uint32_t i;
	char str[1000];

	if (!ldfnode::CheckNodeName(publisher, master, slaves, slaves_count))
	{
		sprintf(str, STR_ERR "Publisher '%s' not defined in database", publisher);
		validation_messages[*validation_messages_count++] = (uint8_t *)strdup(str);
	}

	for (i = 0; i < subscribers_count; i++)
	{
		if (!ldfnode::CheckNodeName(subscribers[i], master, slaves, slaves_count))
		{
			sprintf(str, STR_ERR "Subscriber '%s' not defined in database", subscribers[i]);
			validation_messages[*validation_messages_count++] = (uint8_t *)strdup(str);
		}
	}
}

void ldfsignal::ValidateUnicity(ldfsignal *signal, uint8_t **validation_messages, uint32_t *validation_messages_count)
{
	char str[1000];

	if (strcmp((char *)name, (char *)signal->name) == 0)
	{
		sprintf(str, STR_ERR "Signal '%s' is defined twice", name);
		validation_messages[*validation_messages_count++] = (uint8_t *)str;
	}
}

uint8_t *ldfsignal::GetName()
{
	return name;
}

uint8_t ldfsignal::GetBitSize()
{
	return bit_size;
}



} /* namespace lin */
