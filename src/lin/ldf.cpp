/*
 * ldf.cpp
 *
 *  Created on: 3 jul. 2019
 *      Author: iso9660
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdexcept>
#include <stdlib.h>
#include "ldf.h"
#include "ldfcommon.h"


using namespace std;


namespace lin
{

ldf::ldf(uint8_t *filename)
{
	int32_t c = EOF;

	// Initialize state parameters
	parsing_state = LDF_PARSING_STATE_NONE;
	is_lin_description_file = false;
	group_level = 0;

	// Initialize attributes
	lin_protocol_version = LIN_PROTOCOL_VERSION_NONE;
	lin_language_version = LIN_LANGUAGE_VERSION_NONE;
	lin_speed = 0;
	slaves_count = 0;
	master = NULL;
	slaves_count = 0;
	signals_count = 0;
	frames_count = 0;
	node_attributes_count = 0;

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

	// Validate data
	Validate();
}

ldf::~ldf()
{
	if (master) delete master;
	while (slaves_count > 0) delete slaves[--slaves_count];
	while (signals_count > 0) delete signals[--signals_count];
	while (frames_count > 0) delete frames[--frames_count];
	while (node_attributes_count > 0) delete node_attributes[--node_attributes_count];
}

void ldf::Validate(void)
{
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
	ldfsignal *signal = NULL;
	ldfmasternode *masternode = NULL;
	ldfframe *frame = NULL;
	ldfframesignal *framesignal = NULL;
	ldfconfigurableframe *configurable_frame = NULL;

	switch (parsing_state)
	{

	case LDF_PARSING_STATE_CONFIGURABLE_FRAMES:
		if (group_level == 3)
		{
			configurable_frame = ldfconfigurableframe::FromUdsStatement(statement);
			if (configurable_frame) node_attributes[node_attributes_count - 1]->AddConfigurableFrame(configurable_frame);
		}
		break;

	case LDF_PARSING_STATE_NODES_ATTRIBUTES:
		if (group_level == 1)
		{
			// Isolate node name
			p = strtok((char *)statement, BLANK_CHARACTERS);

			// Add new node attributes
			node_attributes[node_attributes_count++] = new ldfnodeattributes((uint8_t *)p);
		}
		else if (group_level == 2)
		{
			// Update node attributes directly from statement
			node_attributes[node_attributes_count - 1]->UpdateFromLdfStatement(statement);
		}
		break;

	case LDF_PARSING_STATE_FRAMES:
		if (group_level == 1)
		{
			frame = ldfframe::FromLdfStatement(statement);
			if (frame) frames[frames_count++] = frame;
		}
		else if (group_level == 2)
		{
			framesignal = ldfframesignal::FromLdfStatement(statement);
			if (framesignal) frames[frames_count - 1]->AddSignal(framesignal);
		}
		break;

	case LDF_PARSING_STATE_SIGNALS:
		signal = ldfsignal::FromLdfStatement(statement);
		if (signal) signals[signals_count++] = signal;
		break;

	case LDF_PARSING_STATE_NODES:
		// Isolate first token
		p = strtok((char *)statement, ":" BLANK_CHARACTERS);

		if (strcmp(p, "Master") == 0)
		{
			// Go to the next string position
			p += 6;
			while (!(*p)) p++;

			// Parse master node
			masternode = ldfmasternode::FromLdfStatement((uint8_t *)p);
			if (master == NULL && masternode)
				master = masternode;
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
			else if (p && strcmp(p, "\"2.0\"") == 0)
			{
				lin_protocol_version = LIN_PROTOCOL_VERSION_2_0;
			}
		}
		else if (strcmp(p, "LIN_language_version") == 0)
		{
			p = strtok(NULL, "=" BLANK_CHARACTERS);
			if (p && strcmp(p, "\"2.1\"") == 0)
			{
				lin_language_version = LIN_LANGUAGE_VERSION_2_1;
			}
			else if (p && strcmp(p, "\"2.0\"") == 0)
			{
				lin_language_version = LIN_LANGUAGE_VERSION_2_0;
			}
		}
		else if (strcmp(p, "LIN_speed") == 0)
		{
			p = strtok(NULL, "=" BLANK_CHARACTERS);
			if (p)
			{
				lin_speed = atof(p) * 1000;
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

	case LDF_PARSING_STATE_NODES_ATTRIBUTES:
		if (strstr((char *)start, "configurable_frames") == (char *)start)
		{
			parsing_state = LDF_PARSING_STATE_CONFIGURABLE_FRAMES;
		}
		else
		{
			process_statement(start);
		}
		break;

	case LDF_PARSING_STATE_FRAMES:
		process_statement(start);
		break;

	default:
		// Isolate first token
		p = strtok((char *)start, BLANK_CHARACTERS);

		// Analyze group
		if (strcmp(p, "Nodes") == 0)
		{
			parsing_state = LDF_PARSING_STATE_NODES;
		}
		else if (strcmp(p, "Signals") == 0)
		{
			parsing_state = LDF_PARSING_STATE_SIGNALS;
		}
		else if (strcmp(p, "Frames") == 0)
		{
			parsing_state = LDF_PARSING_STATE_FRAMES;
		}
		else if (strcmp(p, "Node_attributes") == 0)
		{
			parsing_state = LDF_PARSING_STATE_NODES_ATTRIBUTES;
		}
		break;

	}
}

void ldf::process_group_end(uint8_t *end)
{
	if (parsing_state == LDF_PARSING_STATE_CONFIGURABLE_FRAMES)
	{
		parsing_state = LDF_PARSING_STATE_NODES_ATTRIBUTES;
	}
	else if (group_level == 1)
	{
		parsing_state = LDF_PARSING_STATE_NONE;
	}
}

} /* namespace ldf */
