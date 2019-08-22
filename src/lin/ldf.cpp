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

ldf::ldf(const uint8_t *filename)
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
	schedule_tables_count = 0;
	encoding_types_count = 0;
	encoding_signals_count = 0;
	validation_messages_count = 0;

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
	while (schedule_tables_count > 0) delete schedule_tables[--schedule_tables_count];
	while (encoding_types_count > 0) delete encoding_types[--encoding_types_count];
	while (encoding_signals_count > 0) delete encoding_signals[--encoding_signals_count];
	while (validation_messages_count > 0) delete validation_messages[--validation_messages_count];
}

void ldf::SortData()
{
	// Sort signals
	qsort(signals, signals_count, sizeof(signals[0]), SorterSignals);
	qsort(frames, frames_count, sizeof(frames[0]), SorterFrames);
}

bool ldf::Validate(void)
{
	uint32_t i, j;

	// Validate results
	if (!is_lin_description_file)
	{
		validation_messages[validation_messages_count++] = (uint8_t *)STR_ERR "Not a LIN definition file";
	}
	if (lin_protocol_version == LIN_PROTOCOL_VERSION_NONE)
	{
		validation_messages[validation_messages_count++] = (uint8_t *)STR_ERR "Protocol version not supported";
	}
	if (lin_language_version == LIN_LANGUAGE_VERSION_NONE)
	{
		validation_messages[validation_messages_count++] = (uint8_t *)STR_ERR "Language version not supported";
	}
	if (lin_speed == 0)
	{
		validation_messages[validation_messages_count++] = (uint8_t *)STR_ERR "LIN speed no defined";
	}
	if (master == NULL)
	{
		validation_messages[validation_messages_count++] = (uint8_t *)STR_ERR "LIN master not found in database";
	}
	if (slaves_count == 0)
	{
		validation_messages[validation_messages_count++] = (uint8_t *)STR_ERR "LIN slaves not found in database";
	}

	// Validate publishers and subscribers of signals
	for (i = 0; i < signals_count; i++)
	{
		signals[i]->ValidateNodes(master, slaves, slaves_count, validation_messages, &validation_messages_count);
	}

	// Validate signals are not repeated
	for (i = 0; i < signals_count; i++)
	{
		for (j = i + 1; j < signals_count; j++)
		{
			signals[i]->ValidateUnicity(signals[j], validation_messages, &validation_messages_count);
		}
	}

	// Validate frames
	for (i = 0; i < frames_count; i++)
	{
		// Validate frame publisher
		frames[i]->ValidatePublisher(master, slaves, slaves_count, validation_messages, &validation_messages_count);

		// Validate frame unicity
		for (j = i + 1; j < frames_count; j++)
		{
			frames[i]->ValidateUnicity(frames[j], validation_messages, &validation_messages_count);
		}

		// Validate frame signals and size
		frames[i]->ValidateSignals(signals, signals_count, validation_messages, &validation_messages_count);
	}

	// Validate node attributes
	for (i = 0; i < node_attributes_count; i++)
	{
		// Validate node name in between slaves
		node_attributes[i]->ValidateNode(slaves, slaves_count, validation_messages, &validation_messages_count);

		// Validate frame unicity
		for (j = i + 1; j < node_attributes_count; j++)
		{
			node_attributes[i]->ValidateUnicity(node_attributes[j], validation_messages, &validation_messages_count);
		}

		// Validate configurable frames
		node_attributes[i]->ValidateFrames(frames, frames_count, validation_messages, &validation_messages_count);
	}

	// Validate schedule tables
	for (i = 0; i < schedule_tables_count; i++)
	{
		// Validate schedule table unicity
		for (j = i + 1; j < schedule_tables_count; j++)
		{
			schedule_tables[j]->ValidateUnicity(schedule_tables[j], validation_messages, &validation_messages_count);
		}

		// Check frames
		schedule_tables[i]->ValidateFrames(frames, frames_count, validation_messages, &validation_messages_count);
	}

	// Validate signal representations
	for (i = 0; i < encoding_signals_count; i++)
	{
		// Check encoding signals unicity
		for (j = i + 1; j < encoding_signals_count; j++)
		{
			encoding_signals[i]->ValidateUnicity(encoding_signals[j], validation_messages, &validation_messages_count);
		}

		// Check signals
		encoding_signals[i]->ValidateSignals(signals, signals_count, validation_messages, &validation_messages_count);
	}

	return validation_messages_count == 0;
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
	ldfschedulecommand *schedule_command = NULL;
	ldfencodingsignals *encoding_signal = NULL;

	switch (parsing_state)
	{

	case LDF_PARSING_STATE_SCHEDULE_TABLES:
		if (group_level == 1)
		{
			p = strtok((char *)statement, BLANK_CHARACTERS);
			if (p) schedule_tables[schedule_tables_count++] = new ldfscheduletable((uint8_t *)p);
		}
		else if (group_level == 2)
		{
			schedule_command = ldfschedulecommand::FromLdfStatement(statement);
			if (schedule_command) schedule_tables[schedule_tables_count - 1]->AddCommand(schedule_command);
		}
		break;

	case LDF_PARSING_STATE_CONFIGURABLE_FRAMES:
		if (group_level == 3)
		{
			configurable_frame = ldfconfigurableframe::FromLdfStatement(statement);
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
		if (group_level == 1)
		{
			signal = ldfsignal::FromLdfStatement(statement);
			if (signal) signals[signals_count++] = signal;
		}
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

	case LDF_PARSING_STATE_ENCODING_TYPES:
		if (group_level == 1)
		{
			p = strtok((char *)statement, BLANK_CHARACTERS);
			if (p) encoding_types[encoding_types_count++] = new ldfencodingtype((uint8_t *)p);
		}
		else if (group_level == 2)
		{
			encoding_types[encoding_types_count - 1]->UpdateFromLdfStatement(statement);
		}
		break;

	case LDF_PARSING_STATE_ENCODING_SIGNALS:
		if (group_level == 1)
		{
			encoding_signal = ldfencodingsignals::FromLdfStatement(statement);
			if (encoding_signal) encoding_signals[encoding_signals_count++] = encoding_signal;
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

	case LDF_PARSING_STATE_SCHEDULE_TABLES:
		process_statement(start);
		break;

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

	case LDF_PARSING_STATE_ENCODING_TYPES:
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
		else if (strcmp(p, "Schedule_tables") == 0)
		{
			parsing_state = LDF_PARSING_STATE_SCHEDULE_TABLES;
		}
		else if (strcmp(p, "Signal_encoding_types") == 0)
		{
			parsing_state = LDF_PARSING_STATE_ENCODING_TYPES;
		}
		else if (strcmp(p, "Signal_representation") == 0)
		{
			parsing_state = LDF_PARSING_STATE_ENCODING_SIGNALS;
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

int ldf::SorterSignals(const void *a, const void *b)
{
	int32_t res = ldfsignal::ComparePublisher(*(const ldfsignal **)a, *(const ldfsignal **)b);
	if (res == 0) res = ldfsignal::Compare(*(const ldfsignal **)a, *(const ldfsignal **)b);
	return res;
}

int ldf::SorterFrames(const void *a, const void *b)
{
	int32_t res = ldfframe::ComparePublisher(*(const ldfframe **)a, *(const ldfframe **)b);
	if (res == 0) res = ldfframe::CompareId(*(const ldfframe **)a, *(const ldfframe **)b);
	return res;
}

void ldf::DeleteSlaveNodeByIndex(uint32_t ix)
{
	delete slaves[ix];
	slaves_count--;
	for (;ix < slaves_count; ix++)
		slaves[ix] = slaves[ix + 1];
}

void ldf::DeleteSlaveNodeAttributesByIndex(uint32_t ix)
{
	delete node_attributes[ix];
	node_attributes_count--;
	for (;ix < node_attributes_count; ix++)
		node_attributes[ix] = node_attributes[ix + 1];
}

void ldf::DeleteSignalByIndex(uint32_t ix)
{
	delete signals[ix];
	signals_count--;
	for (;ix < signals_count; ix++)
		signals[ix] = signals[ix + 1];
}

void ldf::DeleteFrameByIndex(uint32_t ix)
{
	delete frames[ix];
	frames_count--;
	for (;ix < frames_count; ix++)
		frames[ix] = frames[ix + 1];
}

lin_protocol_version_e ldf::GetLinProtocolVersion()
{
	return lin_protocol_version;
}

void ldf::SetLinProtocolVersion(lin_protocol_version_e v)
{
	lin_protocol_version = v;
}

lin_language_version_e ldf::GetLinLanguageVersion()
{
	return lin_language_version;
}

void ldf::SetLinLanguageVersion(lin_language_version_e v)
{
	lin_language_version = v;
}

uint16_t ldf::GetLinSpeed()
{
	return lin_speed;
}

void ldf::SetLinSpeed(uint16_t s)
{
	lin_speed = s;
}

ldfmasternode *ldf::GetMasterNode()
{
	if (master == NULL)
	{
		master = new ldfmasternode((uint8_t *)"", 0, 0);
	}

	return master;
}

ldfnode *ldf::GetSlaveNode(uint32_t ix)
{
	return slaves[ix];



}

uint32_t ldf::GetSlaveNodesCount()
{
	return slaves_count;
}

ldfnodeattributes *ldf::GetSlaveNodeAttributesByName(uint8_t *slave_name)
{
	uint32_t ix;
	ldfnodeattributes *ret = NULL;

	// Look for node attributes
	for (ix = 0; (ret == NULL) && (ix < node_attributes_count); ix++)
	{
		ret = node_attributes[ix]->NameIs(slave_name) ? node_attributes[ix] : NULL;
	}

	return ret;
}

void ldf::AddSlaveNode(ldfnodeattributes *n)
{
	// Add slave and store slave node attributes
	if (slaves_count < sizeof(slaves) / sizeof(slaves[0]))
	{
		slaves[slaves_count++] = new ldfnode(n->GetName());
		node_attributes[node_attributes_count++] = n;
	}
}

void ldf::UpdateSlaveNode(uint8_t *old_slave_name, ldfnodeattributes *n)
{
	// Update slave name
	for (uint32_t ix = 0; ix < slaves_count; ix++)
	{
		// Skip slaves
		if (!slaves[ix]->NameIs(old_slave_name))
			continue;

		// Update slave name
		slaves[ix]->SetName(n->GetName());
		break;
	}

	// Update slave node attributes
	for (uint32_t ix = 0; ix < node_attributes_count; ix++)
	{
		// Skip node attributes
		if (!node_attributes[ix]->NameIs(old_slave_name))
			continue;

		// Replace node attibutes
		delete node_attributes[ix];
		node_attributes[ix] = n;
		break;
	}

	// Update slave node in signals
	for (uint32_t ix = 0; ix < signals_count; ix++)
	{
		signals[ix]->UpdateNodeName(old_slave_name, n->GetName());
	}

	// Update slave node in frames
	for (uint32_t ix = 0; ix < frames_count; ix++)
	{
		frames[ix]->UpdateNodeName(old_slave_name, n->GetName());
	}
}

void ldf::DeleteSlaveNode(uint8_t *slave_name)
{
	// Delete slave name
	for (uint32_t ix = 0; ix < slaves_count; ix++)
	{
		// Skip slaves
		if (!slaves[ix]->NameIs(slave_name))
			continue;

		// Delete slave and move all list one slot back
		DeleteSlaveNodeByIndex(ix);
		break;
	}

	// Delete slave node attributes
	for (uint32_t ix = 0; ix < node_attributes_count; ix++)
	{
		// Skip node attributes
		if (!node_attributes[ix]->NameIs(slave_name))
			continue;

		// Delete node attributes and move all list one slot back
		DeleteSlaveNodeAttributesByIndex(ix);
		break;
	}

	// Delete signals that use the slave
	for (uint32_t ix = 0; ix < signals_count; )
	{
		// Skip signals
		if (!signals[ix]->UsesSlave(slave_name))
		{
			ix++;
			continue;
		}

		// Delete signal and drag all others back
		DeleteSignalByIndex(ix);
	}

	// Delete frames that use the slave
	for (uint32_t ix = 0; ix < frames_count; )
	{
		// Skip frame
		if (!frames[ix]->PublisherIs(slave_name))
		{
			ix++;
			continue;
		}

		// Delete frame and drag all others back
		DeleteFrameByIndex(ix);
	}

	// Remove deleted signals from frames
	for (uint32_t ix = 0; ix < frames_count; ix++)
	{
		for (uint32_t jx = 0; jx < frames[ix]->GetSignalsCount(); )
		{
			// Skip signal
			if (GetSignalByName(frames[ix]->GetSignal(jx)->GetName()))
			{
				jx++;
				continue;
			}

			// Remove signal from frame
			frames[ix]->DeleteSignalByIndex(jx);
		}
	}
}

ldfsignal *ldf::GetSignalByIndex(uint32_t ix)
{
	return signals[ix];
}

ldfsignal *ldf::GetSignalByName(uint8_t *signal_name)
{
	if (signal_name == NULL)
		return NULL;

	for (uint32_t ix = 0; ix < signals_count; ix++)
		if (signals[ix]->NameIs(signal_name))
			return signals[ix];

	return NULL;
}

uint32_t ldf::GetSignalsCount()
{
	return signals_count;
}

void ldf::AddSignal(ldfsignal *s)
{
	signals[signals_count++] = s;
}

void ldf::UpdateSignal(uint8_t *old_signal_name, ldfsignal *s)
{
	if (old_signal_name == NULL)
		return;

	// Update signal with the new one
	for (uint32_t ix = 0; ix < signals_count; ix++)
	{
		// Look for signal by ID
		if (!signals[ix]->NameIs(old_signal_name))
			continue;

		// Delete signal
		delete signals[ix];
		signals[ix] = s;
		break;
	}

	// Update signal name in frames
	for (uint32_t ix = 0; ix < frames_count; ix++)
	{
		frames[ix]->UpdateSignalName(old_signal_name, s->GetName());
	}
}

void ldf::DeleteSignal(uint8_t *signal_name)
{
	if (signal_name == NULL)
		return;

	// Delete signal from signals
	for (uint32_t ix = 0; ix < signals_count; ix++)
	{
		// Look for signal by ID
		if (!signals[ix]->NameIs(signal_name))
			continue;

		// Delete signal
		DeleteSignalByIndex(ix);
		break;
	}

	// Delete signal from frames
	for (uint32_t ix = 0; ix < frames_count; ix++)
	{
		frames[ix]->DeleteSignalByName(signal_name);
	}
}

void ldf::UpdateMasterNodeName(uint8_t *old_name, uint8_t *new_name)
{
	// Update node name in signals
	for (uint32_t ix = 0; ix < signals_count; ix++)
	{
		signals[ix]->UpdateNodeName(old_name, new_name);
	}

	// Update master node name
	master->SetName(new_name);
}

ldfframe *ldf::GetFrameByIndex(uint32_t ix)
{
	return frames[ix];
}

ldfframe *ldf::GetFrameByName(uint8_t *frame_name)
{
	if (frame_name == NULL)
		return NULL;

	for (uint32_t ix = 0; ix < frames_count; ix++)
		if (frames[ix]->NameIs(frame_name))
			return frames[ix];

	return NULL;
}

ldfframe *ldf::GetFrameById(uint8_t frame_id)
{
	for (uint32_t ix = 0; ix < frames_count; ix++)
		if (frames[ix]->GetId() == frame_id)
			return frames[ix];

	return NULL;
}

uint32_t ldf::GetFramesCount()
{
	return frames_count;
}

void ldf::AddFrame(ldfframe *f)
{
	frames[frames_count++] = f;
}

void ldf::UpdateFrame(uint8_t *old_frame_name, ldfframe *f)
{
	// Update frame
	for (uint32_t ix = 0; ix < frames_count; ix++)
	{
		// Skip frames
		if (!frames[ix]->NameIs(old_frame_name))
			continue;

		// Replace frame in index
		delete frames[ix];
		frames[ix] = f;
		break;
	}

	// TODO:: Update frame in schedule tables
}

void ldf::DeleteFrame(uint8_t *frame_name)
{
	// Delete frame
	for (uint32_t ix = 0; ix < frames_count; ix++)
	{
		// Skip frames
		if (!frames[ix]->NameIs(frame_name))
			continue;

		// Delete frame and shuffle the rest ones
		DeleteFrameByIndex(ix);
		break;
	}

	// TODO:: Delete frame from schedule table
}


} /* namespace ldf */
