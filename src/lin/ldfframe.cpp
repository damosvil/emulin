/*
 * ldfframe.cpp
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ldfcommon.h>
#include <ldfframe.h>


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

uint8_t *ldfframe::GetName()
{
	return name;
}

void ldfframe::ValidatePublisher(ldfnode *master, ldfnode **slaves, uint32_t slaves_count, uint8_t **validation_messages, uint32_t *validation_messages_count)
{
	char str[1000];

	// Check publisher node exists
	if (!ldfnode::CheckNodeName(publisher, master, slaves, slaves_count))
	{
		sprintf(str, STR_ERR "Publisher node '%s' assigned to frame '%s' is not defined.", publisher, name);
		validation_messages[*validation_messages_count++] = (uint8_t *)strdup(str);
	}
}

void ldfframe::ValidateUnicity(ldfframe *frame, uint8_t **validation_messages, uint32_t *validation_messages_count)
{
	char str[1000];

	if (strcmp((char *)name, (char *)frame->name) == 0)
	{
		sprintf(str, STR_ERR "Frame name '%s' used in two different frame definitions.", name);
		validation_messages[*validation_messages_count++] = (uint8_t *)strdup(str);
	}

	if (id == frame->id)
	{
		sprintf(str, STR_ERR "Frame ID 0x'%X' used in two different frames: '%s' and '%s'.", id, name, frame->name);
		validation_messages[*validation_messages_count++] = (uint8_t *)strdup(str);
	}
}

void ldfframe::Validate(ldfsignal **signals, uint32_t signals_count, uint8_t **validation_messages, uint32_t *validation_messages_count)
{
	bool overlapping;
	uint32_t i, j;
	char str[1000];
	bool frame_bitmap[64];
	uint32_t frame_bit_length = 8 * this->size;

	// Check frame size
	if (this->size > 8)
	{
		sprintf(str, STR_ERR "Frame '%s' size %d incorrect.", this->name, this->size);
		validation_messages[*validation_messages_count++] = (uint8_t *)strdup(str);
		return;
	}

	// Check frame signals integrity
	memset(frame_bitmap, 0, sizeof(frame_bitmap));
	for (i = 0; i < this->signals_count; i++)
	{
		ldfsignal *s = NULL;

		// Look for signal definition
		for (j = 0; (s == NULL) && (j < signals_count); j++)
		{
			if (strcmp((char *)this->signals[i]->GetName(), (char *)signals[j]->GetName()) == 0)
			{
				s = signals[j];
			}
		}

		// Check signal is defined
		if (s == NULL)
		{
			sprintf(str, STR_ERR "Signal '%s' used in frame '%s' not defined.", this->signals[i]->GetName(), name);
			validation_messages[*validation_messages_count++] = (uint8_t *)strdup(str);
			continue;
		}

		// Check signal name for repetition
		for (j = i + 1; j < this->signals_count; j++)
		{
			if (strcmp((char *)this->signals[i]->GetName(), (char *)this->signals[j]->GetName()) == 0)
			{
				sprintf(str, STR_ERR "Signal name '%s' used twice in frame '%s'.", this->signals[i]->GetName(), name);
				validation_messages[*validation_messages_count++] = (uint8_t *)strdup(str);
			}
		}

		// Check signal boundaries
		if (this->signals[i]->GetOffset() + s->GetBitSize() > frame_bit_length)
		{
			sprintf(str, STR_ERR "Signal '%s' used in frame '%s' outside of frame boundaries.", this->signals[i]->GetName(), name);
			validation_messages[*validation_messages_count++] = (uint8_t *)strdup(str);
			continue;
		}

		// Update frame bitmap with signal
		overlapping = false;
		for (j = 0; j < s->GetBitSize(); j++)
		{
			// Update overlapping
			overlapping = overlapping || frame_bitmap[this->signals[i]->GetOffset() + j];

			// Set bit in frame_bitmap
			frame_bitmap[this->signals[i]->GetOffset() + j] = true;
		}

		// Check signal overlapping
		if (overlapping)
		{
			sprintf(str, STR_ERR "Signal '%s' used in frame '%s' overlapping previous signal.", this->signals[i]->GetName(), name);
			validation_messages[*validation_messages_count++] = (uint8_t *)strdup(str);
		}
	}
}


} /* namespace lin */
