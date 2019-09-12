/*
 * ldfsignalencoding.cpp
 *
 *  Created on: 15 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <ldfcommon.h>
#include <ldfencodingtype.h>

namespace lin {

ldfencodingtype::ldfencodingtype(const uint8_t *name)
{
	this->name = StrDup(name);
	treat_as_bcd = false;
	treat_as_ascii = false;
	physical_value = NULL;
	logical_values_count = 0;
}

ldfencodingtype::~ldfencodingtype()
{
	if (name) delete name;
	if (physical_value) delete physical_value;
	while (logical_values_count > 0) delete logical_values[--logical_values_count];
}

void ldfencodingtype::UpdateFromLdfStatement(uint8_t *statement)
{
	// Type name
	char *p = strtok((char *)statement, "," BLANK_CHARACTERS);

	// Parse the rest
	if (StrEq(p, "logical_value"))
	{
		logical_values[logical_values_count] = ldflogicalvalue::FromLdfStatement(Str(strtok(NULL, "")));
		if (logical_values[logical_values_count]) logical_values_count++;
	}
	else if (StrEq(p, "physical_value"))
	{
		if (physical_value == NULL)
		{
			physical_value = ldfphysicalvalue::FromLdfStatement(Str(strtok(NULL, "")));
		}
	}
	else if (StrEq(p, "bcd_value"))
	{
		treat_as_bcd = true;
	}
	else if (StrEq(p, "ascii_value"))
	{
		treat_as_ascii = true;
	}
}

} /* namespace lin */
