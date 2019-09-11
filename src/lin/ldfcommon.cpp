/*
 * ldfcommon.c
 *
 *  Created on: 8 ago. 2019
 *      Author: iso9660
 */

#include <stdlib.h>
#include "ldfcommon.h"
#include "tools.h"

using namespace tools;

static const char *lin_protocol_ids[] = { NULL, "1", "0" };
static const char *lin_language_ids[] = { NULL, "1", "0" };

namespace lin
{

const char *GetLinProtocolVersionStringID(lin_protocol_version_e v)
{
	return lin_protocol_ids[v];
}

const char *GetLinLanguageVersionStringID(lin_language_version_e v)
{
	return lin_language_ids[v];
}

lin_protocol_version_e GetProtocolVersionByStringID(const char *id)
{
	return (lin_protocol_version_e)GetStrIndexByID(lin_protocol_ids, ARR_SIZE(lin_protocol_ids), id);
}

lin_language_version_e GetLanguageVersionByStringID(const char *id)
{
	return (lin_language_version_e)GetStrIndexByID(lin_language_ids, ARR_SIZE(lin_language_ids), id);
}

int32_t ParseInt(const char *p)
{
	return (p[1] == 'x' || p[1] == 'X') ? strtol(p, NULL, 16) : atoi(p);
}

bool StrEq(const uint8_t *a, const uint8_t *b)
{
	return strcmp((char *)a, (char *)b) == 0;
}

bool StrEq(const uint8_t *a, const char *b)
{
	return StrEq((const uint8_t *)a, (const uint8_t *)b);
}

bool StrEq(const char *a, const uint8_t *b)
{
	return StrEq((const uint8_t *)a, (const uint8_t *)b);
}

bool StrEq(const char *a, const char *b)
{
	return StrEq((const uint8_t *)a, (const uint8_t *)b);
}

uint8_t *StrDup(const char *a)
{
	return (uint8_t *)strdup(a);
}

uint8_t *StrDup(const uint8_t *a)
{
	return StrDup((const char *)a);
}



}
