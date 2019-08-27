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

}
