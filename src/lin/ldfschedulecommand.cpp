/*
 * ldfschedulecommand.cpp
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ldfcommon.h>
#include <ldfschedulecommand.h>

namespace lin {

ldfschedulecommand::ldfschedulecommand(ldfschedulecommandtype_t type, uint8_t *frame_name, uint16_t timeout, uint8_t *slave, uint8_t *data, uint8_t *assign_frame)
{
	this->type = type;
	this->frame_name = (uint8_t *)strdup((char *) frame_name);
	this->timeout = timeout;
	this->slave = (uint8_t *)strdup((char *) slave);
	this->data = (uint8_t *)strdup((char *) data);
	this->assign_frame = (uint8_t *)strdup((char *) assign_frame);
}

ldfschedulecommand::~ldfschedulecommand()
{
	if (frame_name) delete frame_name;
}

ldfschedulecommand *ldfschedulecommand::FromLdfStatement(uint8_t *statement)
{
	char *p;
	ldfschedulecommandtype_t type;
	char *frame_name = NULL;
	uint16_t timeout = 0;
	uint8_t *slave = NULL;
	uint8_t *data = NULL;
	uint8_t *assign_frame = NULL;

	// Frame name
	p = strtok((char *) statement, BLANK_CHARACTERS);
	if (p == NULL) return NULL;
	frame_name = p;

	if (strcmp(frame_name, "MasterReq") == 0)
	{
		type = LDF_SCMD_TYPE_MarterReq;
	}
	else if (strcmp(frame_name, "SlaveResp") == 0)
	{
		type = LDF_SCMD_TYPE_SlaveResp;
	}
	else if (strcmp(frame_name, "AssignNAD") == 0)
	{
		type = LDF_SCMD_TYPE_AssignNAD;
	}
	else if (strcmp(frame_name, "DataDump") == 0)
	{
		type = LDF_SCMD_TYPE_DataDump;
	}
	else if (strcmp(frame_name, "SaveConfig") == 0)
	{
		type = LDF_SCMD_TYPE_SaveConfig;
	}
	else if (strcmp(frame_name, "FreeFormat") == 0)
	{
		type = LDF_SCMD_TYPE_FreeFormat;
	}
	else if (strcmp(frame_name, "AssignFrameIdRange") == 0)
	{
		type = LDF_SCMD_TYPE_AssignFrameIdRange;
	}
	else if (strcmp(frame_name, "AssignFrameId") == 0)
	{
		type = LDF_SCMD_TYPE_AssignFrameId;
	}
	else
	{
		type = LDF_SCMD_TYPE_UnconditionalFrame;
	}

	// TODO: Parse diagnostic frames

	// Check delay word
	p = strtok(NULL, BLANK_CHARACTERS);
	if (p == NULL) return NULL;
	if (strcmp(p, "delay") != 0) return NULL;

	// Timeout
	p = strtok(NULL, BLANK_CHARACTERS);
	if (p == NULL) return NULL;
	timeout = atof(p);

	// Check ms word
	p = strtok(NULL, BLANK_CHARACTERS);
	if (p == NULL) return NULL;
	if (strcmp(p, "ms") != 0) return NULL;

	// Return command
	return new ldfschedulecommand(type, (uint8_t *)frame_name, timeout, slave, data, assign_frame);
}

uint8_t *ldfschedulecommand::GetName()
{
	return frame_name;
}

uint16_t ldfschedulecommand::GetTimeoutMs()
{
	return timeout;
}

void ldfschedulecommand::UpdateName(const uint8_t *old_name, const uint8_t *new_name)
{
	if (strcmp((char *)frame_name, (char *)old_name) == 0)
	{
		delete frame_name;
		frame_name = (uint8_t *)strdup((char *)new_name);
	}
}

void ldfschedulecommand::ValidateUnicity(uint8_t *schedule_table, ldfschedulecommand *command, uint8_t **validation_messages, uint32_t *validation_messages_count)
{
	char str[1000];

	if (strcmp((char *)frame_name, (char *)command->frame_name) == 0)
	{
		sprintf(str, STR_ERR "Schedule table '%s' schedule command frame name '%s' repeated.", schedule_table, frame_name);
		validation_messages[*validation_messages_count++] = (uint8_t *)strdup(str);
	}
}


} /* namespace lin */
