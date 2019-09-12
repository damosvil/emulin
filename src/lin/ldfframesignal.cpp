/*
 * ldfframesignal.cpp
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ldfcommon.h>
#include <ldfframesignal.h>


namespace lin {

ldfframesignal::ldfframesignal(const uint8_t *name, uint16_t offset)
{
	this->name = StrDup(name);
	this->offset = offset;
}

ldfframesignal::~ldfframesignal()
{
	if (name) delete name;
}

ldfframesignal *ldfframesignal::FromLdfStatement(uint8_t *statement)
{
	char *p = NULL;
	char *name = NULL;
	uint16_t offset = 0;

	// Name
	p = strtok((char *)statement, ":," BLANK_CHARACTERS);
	if (p) name = p;

	// ID
	if (p) p = strtok(NULL, ":," BLANK_CHARACTERS);
	if (p) offset = ParseInt(p);

	if (name)
	{
		return new ldfframesignal(Str(name), offset);
	}
	else
	{
		return NULL;
	}
}

int32_t ldfframesignal::SorterFrameSignals(const void *a, const void *b)
{
	ldfframesignal *aa = *(ldfframesignal **)a;
	ldfframesignal *bb = *(ldfframesignal **)b;

	int res = aa->offset - bb->offset;
	if (res == 0)
	{
		res = strcmp((char *)aa->name, (char *)bb->name);
	}

	return res;
}

uint8_t *ldfframesignal::GetName()
{
	return name;
}

uint16_t ldfframesignal::GetOffset()
{
	return offset;
}

void ldfframesignal::SetName(const uint8_t *name)
{
	delete this->name;
	this->name = StrDup(name);
}

void ldfframesignal::SetOffset(uint16_t offset)
{
	this->offset = offset;
}

bool ldfframesignal::NameIs(const uint8_t *name)
{
	return StrEq(name, this->name);
}


} /* namespace lin */
