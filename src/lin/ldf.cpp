/*
 * ldf.cpp
 *
 *  Created on: 3 jul. 2019
 *      Author: iso9660
 */

#include "ldf.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdexcept>
#include <stdlib.h>

using namespace std;

#define BLANK_CHARACTERS					" \t\r\n"


namespace lin
{

ldf::ldf(uint8_t *filename)
{
	int32_t c = EOF;

	// Initialize all parameters
	parsing_state = LDF_PARSING_STATE_NONE;
	is_lin_description_file = false;
	group_level = 0;
	lin_protocol_version = LIN_PROTOCOL_VERSION_NONE;
	lin_language_version = LIN_LANGUAGE_VERSION_NONE;
	lin_speed = 0;
	slaves_count = 0;
	master = NULL;
	slaves_count = 0;

	// Open file
	FILE *ldf_file = fopen((char *)filename, "rb");
	if (!ldf_file)
	{
		throw runtime_error("Filename does not exist");
	}

	// Parse file by character
	while ((c = getc(ldf_file)) != EOF)
	{
		parse_char(c);
	}

	// Close file
	fclose(ldf_file);

	// Validate results
	if (!is_lin_description_file)
	{
		throw runtime_error("Not a LIN definition file");
	}
	else if (lin_protocol_version == LIN_PROTOCOL_VERSION_NONE)
	{
		throw runtime_error("Protocol version not supported");
	}
	else if (lin_language_version == LIN_LANGUAGE_VERSION_NONE)
	{
		throw runtime_error("Language version not supported");
	}
	else if (lin_speed == 0)
	{
		throw runtime_error("LIN speed no defined");
	}
	else if (master == NULL)
	{
		throw runtime_error("LIN master not found in database");
	}
	else if (slaves_count == 0)
	{
		throw runtime_error("LIN slaves not found in database");
	}
}

ldf::~ldf()
{
	uint8_t i;

	if (master) delete master;
	for (i = 0; i < slaves_count; i++) delete slaves[i];
	slaves_count = 0;
}

bool ldf::char_in_set(uint8_t c, const char *set)
{
	for (const char *p = set; *p; p++)
		if (c == *p)
			return true;

	return false;
}

void ldf::parse_char(uint8_t c)
{
	static uint8_t line[10000];
	static uint32_t line_length = 0;
	static bool skip = false;

	if (c == ';')
	{
		line[line_length] = 0;
		process_statement(line);
		line[0] = 0;
		line_length = 0;
	}
	else if (c == '{')
	{
		line[line_length] = 0;
		process_group_start(line);
		line[0] = 0;
		line_length = 0;
		group_level++;
	}
	else if (c == '}')
	{
		line[line_length] = 0;
		process_group_end(line);
		line[0] = 0;
		line_length = 0;
		group_level--;
	}
	else if (c == '/')
	{
		// Skip comments
		skip = true;
	}
	else if (c == '\n')
	{
		// Comments are valid to the end of the line
		skip = false;
	}
	else if (!skip && (line_length != 0 || !char_in_set(c, BLANK_CHARACTERS)))
	{
		if (line_length < sizeof(line) - 1)
		{
			line[line_length++] = c;
		}
	}
}

void ldf::process_statement(uint8_t *statement)
{
	char *p = NULL;
	char *name = NULL;
	uint16_t timebase = 0;
	uint16_t jitter = 0;

	switch (parsing_state)
	{

	case LDF_PARSING_STATE_NODES:
		// Isolate first token
		p = strtok((char *)statement, ":" BLANK_CHARACTERS);

		if (strcmp(p, "Master") == 0)
		{
			// Name
			p = strtok(NULL, "," BLANK_CHARACTERS);
			if (p) name = p;

			// Timebase
			if (p) p = strtok(NULL, "," BLANK_CHARACTERS);
			if (p) timebase = atoi(p);

			// Jitter
			if (p) p = strtok(NULL, "," BLANK_CHARACTERS);	// Skip word ms
			if (p) p = strtok(NULL, "," BLANK_CHARACTERS);
			if (p) jitter = atof(p) * 10;

			// Add master
			if (master == NULL && name != NULL)
				master = new ldfmasternode((uint8_t *)name, timebase, jitter);
		}
		else if (strcmp(p, "Slaves") == 0)
		{
			while (p)
			{
				p = strtok(NULL, "," BLANK_CHARACTERS);
				if (p) slaves[slaves_count++] = new ldfnode((uint8_t *)p);
			}
		}
		break;

	case LDF_PARSING_STATE_NONE:
	default:
		// Isolate first token
		p = strtok((char *)statement, "=" BLANK_CHARACTERS);

		// Check token
		if (strcmp(p, "LIN_description_file") == 0)
		{
			is_lin_description_file = true;
		}
		else if (strcmp(p, "LIN_protocol_version") == 0)
		{
			p = strtok(NULL, "=" BLANK_CHARACTERS);
			if (p && strcmp(p, "\"2.1\"") == 0)
			{
				lin_protocol_version = LIN_PROTOCOL_VERSION_2_1;
			}
		}
		else if (strcmp(p, "LIN_language_version") == 0)
		{
			p = strtok(NULL, "=" BLANK_CHARACTERS);
			if (p && strcmp(p, "\"2.1\"") == 0)
			{
				lin_language_version = LIN_LANGUAGE_VERSION_2_1;
			}
		}
		else if (strcmp(p, "LIN_speed") == 0)
		{
			p = strtok(NULL, "=" BLANK_CHARACTERS);
			if (p && strcmp(p, "19.2") == 0)
			{
				lin_speed = 19200;
			}
			else if (p && strcmp(p, "9.6") == 0)
			{
				lin_speed = 9600;
			}
		}
		break;
	}
}

void ldf::process_group_start(uint8_t *start)
{
	char *p;

	switch (parsing_state)
	{

	case LDF_PARSING_STATE_NONE:
	default:
		p = strtok((char *)start, BLANK_CHARACTERS);
		if (strcmp(p, "Nodes") == 0)
		{
			parsing_state = LDF_PARSING_STATE_NODES;
		}
		else if (strcmp(p, "Signals") == 0)
		{
			parsing_state = LDF_PARSING_STATE_SIGNALS;
		}
		break;

	}
}

void ldf::process_group_end(uint8_t *end)
{
	if (group_level == 1)
		parsing_state = LDF_PARSING_STATE_NONE;
}

} /* namespace ldf */
