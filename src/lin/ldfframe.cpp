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

ldfframe::ldfframe(const uint8_t *name, uint8_t id, const uint8_t *publisher, uint8_t size)
{
	this->name = StrDup(name);
	this->id = id;
	this->publisher = StrDup(publisher);
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
	if (p) id = ParseInt(p);

	// Publisher
	if (p) p = strtok(NULL, ":," BLANK_CHARACTERS);
	if (p) publisher = p;

	// Frame size
	if (p) p = strtok(NULL, ":," BLANK_CHARACTERS);
	if (p) frame_size = ParseInt(p);

	// Validate and return a new frame
	if (name != NULL && publisher != NULL && id != 0xFF && frame_size != 0)
	{
		return new ldfframe(Str(name), id, Str(publisher), frame_size);
	}
	else
	{
		return NULL;
	}
}

uint8_t *ldfframe::GetName()
{
	return name;
}

uint8_t ldfframe::GetId()
{
	return id;
}

uint8_t ldfframe::GetPid()
{
	uint8_t p1 = 0x01 & (id ^ (id >> 1) ^ (id >> 2) ^ (id >> 4));
	uint8_t p2 = 0x01 & ~((id >> 1) ^ (id >> 3) ^ (id >> 4) ^ (id >> 5));
	return id | (p1 << 6) | (p2 << 7);
}

uint8_t *ldfframe::GetPublisher()
{
	return publisher;
}

uint8_t ldfframe::GetSize()
{
	return size;
}

ldfframesignal *ldfframe::GetSignal(uint32_t ix)
{
	return signals[ix];
}

uint32_t ldfframe::GetSignalsCount()
{
	return signals_count;
}

void ldfframe::AddSignal(ldfframesignal *signal)
{
	signals[signals_count++] = signal;
}

bool ldfframe::NameIs(const uint8_t *name)
{
	return StrEq(name, this->name);
}

bool ldfframe::PublisherIs(const uint8_t *publisher)
{
	return StrEq(publisher, this->publisher);
}

void ldfframe::DeleteSignalByIndex(uint32_t ix)
{
	delete signals[ix];
	signals_count--;
	for (; ix < signals_count; ix++) signals[ix] = signals[ix + 1];
}

void ldfframe::DeleteSignalByName(const uint8_t *signal_name)
{
	for (uint32_t ix = 0; ix < signals_count; ix++)
	{
		// Search signal
		if (!signals[ix]->NameIs(signal_name))
			continue;

		// Move back all signals one place
		DeleteSignalByIndex(ix);
		break;
	}
}

void ldfframe::UpdateSignalName(const uint8_t *old_signal_name, const uint8_t *new_signal_name)
{
	for (uint32_t ix = 0; ix < signals_count; ix++)
	{
		// Search signal
		if (!signals[ix]->NameIs(old_signal_name))
			continue;

		// Move back all signals one place
		signals[ix]->SetName(new_signal_name);
		break;
	}
}

void ldfframe::UpdateNodeName(const uint8_t *old_name, const uint8_t *new_name)
{
	if (StrEq(old_name, publisher))
		return;

	delete publisher;
	publisher = StrDup(new_name);
}

void ldfframe::ValidatePublisher(ldfnode *master, ldfnode **slaves, uint32_t slaves_count, uint8_t **validation_messages, uint32_t *validation_messages_count)
{
	char str[1000];

	// Check publisher node exists
	if (!ldfnode::CheckNodeName(publisher, master, slaves, slaves_count))
	{
		sprintf(str, STR_ERR "Publisher node '%s' assigned to frame '%s' is not defined.", publisher, name);
		validation_messages[*validation_messages_count++] = StrDup(str);
	}
}

void ldfframe::ValidateUnicity(ldfframe *frame, uint8_t **validation_messages, uint32_t *validation_messages_count)
{
	char str[1000];

	if (StrEq(name, frame->name))
	{
		sprintf(str, STR_ERR "Frame name '%s' used in two different frame definitions.", name);
		validation_messages[*validation_messages_count++] = StrDup(str);
	}

	if (id == frame->id)
	{
		sprintf(str, STR_ERR "Frame ID 0x'%X' used in two different frames: '%s' and '%s'.", id, name, frame->name);
		validation_messages[*validation_messages_count++] = StrDup(str);
	}
}

void ldfframe::ValidateSignals(ldfsignal **signals, uint32_t signals_count, uint8_t **validation_messages, uint32_t *validation_messages_count)
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
		validation_messages[(*validation_messages_count)++] = StrDup(str);
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
			if (this->signals[i]->NameIs(signals[j]->GetName()))
			{
				s = signals[j];
			}
		}

		// Check signal is defined
		if (s == NULL)
		{
			sprintf(str, STR_ERR "Signal '%s' used in frame '%s' not defined.", this->signals[i]->GetName(), name);
			validation_messages[(*validation_messages_count)++] = StrDup(str);
			continue;
		}

		// Check signal name for repetition
		for (j = i + 1; j < this->signals_count; j++)
		{
			if (this->signals[i]->NameIs(this->signals[j]->GetName()))
			{
				sprintf(str, STR_ERR "Signal name '%s' used twice in frame '%s'.", this->signals[i]->GetName(), name);
				validation_messages[(*validation_messages_count)++] = StrDup(str);
			}
		}

		// Check signal boundaries
		if (this->signals[i]->GetOffset() + s->GetBitSize() > frame_bit_length)
		{
			sprintf(str, STR_ERR "Signal '%s' used in frame '%s' outside of frame boundaries.", this->signals[i]->GetName(), name);
			validation_messages[(*validation_messages_count)++] = StrDup(str);
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
			validation_messages[(*validation_messages_count)++] = StrDup(str);
		}
	}
}

int32_t ldfframe::CompareId(const ldfframe *a, const ldfframe *b)
{
	return a->id - b->id;
}

int32_t ldfframe::ComparePublisher(const ldfframe *a, const ldfframe *b)
{
	if (a->publisher == NULL) return -1;
	if (b->publisher == NULL) return 1;
	return strcmp((char *)a->publisher, (char *)b->publisher);
}

void ldfframe::SortData()
{
	qsort(signals, signals_count, sizeof(signals[0]), ldfframesignal::SorterFrameSignals);
}


} /* namespace lin */
