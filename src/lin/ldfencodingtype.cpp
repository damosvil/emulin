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

ldfencodingtype::ldfencodingtype(uint8_t *name)
{
	this->name = (uint8_t *)strdup((char *)name);
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
	if (strcmp(p, "logical_value") == 0)
	{
		logical_values[logical_values_count] = ldflogicalvalue::FromLdfStatement((uint8_t *) strtok(NULL, ""));
		if (logical_values[logical_values_count]) logical_values_count++;
	}
	else if (strcmp(p, "physical_value") == 0)
	{
		if (physical_value == NULL)
		{
			physical_value = ldfphysicalvalue::FromLdfStatement((uint8_t *) strtok(NULL, ""));
		}
	}
	else if (strcmp(p, "bcd_value") == 0)
	{
		treat_as_bcd = true;
	}
	else if (strcmp(p, "ascii_value") == 0)
	{
		treat_as_ascii = true;
	}
}

} /* namespace lin */
