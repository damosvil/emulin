/*
 * ldf.cpp
 *
 *  Created on: 3 jul. 2019
 *      Author: ctag
 */

#include "ldf.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdexcept>
#include <stdlib.h>

using namespace std;

#define TOKEN_SPLITTER					" =:;,\r\n\t"


namespace lin
{

ldf::ldf(uint8_t *filename)
{
	char *p = NULL;
	char line[1000];

	// Initialize all parameters
	parsing_state = LDF_PARSING_STATE_NONE;
	is_lin_description_file = false;
	bracket_level = 0;
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

	// Parse file line by line
	while (fgets(line, sizeof(line), ldf_file))
	{
		// Convert string to uppercase
		for (p = line; *p; p++) *p = toupper(*p);

		// String tokenization
		p = strtok(line, TOKEN_SPLITTER);

		// Skip comment lines or empty lines
		if (!p || *p == '/' || *p == 0) continue;

		// Parsing first argument
		if (strcmp(p, "{") == 0)
		{
			bracket_level++;
		}
		else if (strcmp(p, "}") == 0)
		{
			if (bracket_level)
				bracket_level--;

			if (bracket_level == 0)
			{
				parsing_state = LDF_PARSING_STATE_NONE;
			}
		}
		else if (parsing_state == LDF_PARSING_STATE_NODES)
		{
			parse_state_nodes(p);
		}
		else
		{
			parse_state_none(p);
		}
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

void ldf::parse_state_none(char *token)
{
	if (strcmp(token, "LIN_DESCRIPTION_FILE") == 0)
	{
		is_lin_description_file = true;
	}
	else if (strcmp(token, "LIN_PROTOCOL_VERSION") == 0)
	{
		token = strtok(NULL, TOKEN_SPLITTER);
		if (token && strcmp(token, "\"2.1\"") == 0)
		{
			lin_protocol_version = LIN_PROTOCOL_VERSION_2_1;
		}
	}
	else if (strcmp(token, "LIN_LANGUAGE_VERSION") == 0)
	{
		token = strtok(NULL, TOKEN_SPLITTER);
		if (token && strcmp(token, "\"2.1\"") == 0)
		{
			lin_language_version = LIN_LANGUAGE_VERSION_2_1;
		}
	}
	else if (strcmp(token, "LIN_SPEED") == 0)
	{
		token = strtok(NULL, TOKEN_SPLITTER);
		if (token && strcmp(token, "19.2") == 0)
		{
			lin_speed = 19200;
		}
		else if (token && strcmp(token, "9.6") == 0)
		{
			lin_speed = 9600;
		}
	}
	else if (strcmp(token, "NODES") == 0)
	{
		parsing_state = LDF_PARSING_STATE_NODES;
	}
	else if (strcmp(token, "SIGNALS") == 0)
	{
		parsing_state = LDF_PARSING_STATE_SIGNALS;
	}
}

void ldf::parse_state_nodes(char *token)
{
	char name[1000];
	uint16_t timebase;
	float jitter;

	if (strcmp(token, "MASTER") == 0)
	{
		// Name
		token = strtok(NULL, TOKEN_SPLITTER);
		if (token) strcpy(name, token);

		// Timebase
		if (token) token = strtok(NULL, TOKEN_SPLITTER);
		if (token) timebase = atoi(token);

		// Jitter
		if (token) token = strtok(NULL, TOKEN_SPLITTER);		// Skip ms
		if (token) token = strtok(NULL, TOKEN_SPLITTER);
		if (token) jitter = (uint16_t)(atof(token) * 10);

		// Add master
		if (master == NULL)
		{
			master = new ldfmasternode((uint8_t *)name, timebase, jitter);
		}
	}
	else if (strcmp(token, "SLAVES") == 0)
	{
		// Add slaves
		while (token)
		{
			token = strtok(NULL, TOKEN_SPLITTER);
			if (token) slaves[slaves_count++] = new ldfnode((uint8_t *)token);
		}
	}
}

void ldf::parse_state_signals(char *token)
{

}



} /* namespace ldf */
