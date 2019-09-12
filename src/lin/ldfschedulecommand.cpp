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
#include <ldf.h>

namespace lin {


ldfschedulecommand::ldfschedulecommand(ldfschedulecommandtype_t type, const uint8_t *frame_name, uint16_t timeout, const uint8_t *slave_name, const uint8_t *data, const uint8_t *assign_frame_name)
{
	this->type = type;
	this->frame_name = StrDup(frame_name);
	this->timeout = timeout;
	this->slave_name = (slave_name != NULL) ? StrDup(slave_name) : NULL;
	for (uint32_t i = 0; i < 8 ; i++) this->data[i] = data[i];
	this->assign_frame_name = (assign_frame_name != NULL) ? StrDup(assign_frame_name) : NULL;
}

ldfschedulecommand::~ldfschedulecommand()
{
	if (frame_name) delete frame_name;
	if (slave_name) delete slave_name;
	if (assign_frame_name) delete assign_frame_name;
}

ldfschedulecommand *ldfschedulecommand::FromLdfStatement(const uint8_t *statement)
{
	char *p;
	ldfschedulecommandtype_t type;
	char *frame_name = NULL;
	uint16_t timeout = 0;
	char *slave = NULL;
	char *assign_frame = NULL;
	char data[8];

	// Frame name
	p = strtok((char *) statement, BLANK_CHARACTERS);
	if (p == NULL) return NULL;
	frame_name = p;

	if (StrEq(frame_name, "MasterReq"))
	{
		type = LDF_SCMD_TYPE_MasterReq;
	}
	else if (StrEq(frame_name, "SlaveResp"))
	{
		type = LDF_SCMD_TYPE_SlaveResp;
	}
	else if (StrEq(frame_name, "AssignNAD"))
	{
		type = LDF_SCMD_TYPE_AssignNAD;

		// Read open bracket
		p = strtok(NULL, BLANK_CHARACTERS);
		if (p == NULL) return NULL;
		if (!StrEq(p, "{")) return NULL;

		// Read slave name
		p = strtok(NULL, BLANK_CHARACTERS);
		if (p == NULL) return NULL;
		slave = p;

		// Read close bracket
		p = strtok(NULL, BLANK_CHARACTERS);
		if (p == NULL) return NULL;
		if (!StrEq(p, "}")) return NULL;
	}
	else if (StrEq(frame_name, "DataDump"))
	{
		type = LDF_SCMD_TYPE_DataDump;

		// Read open bracket
		p = strtok(NULL, BLANK_CHARACTERS);
		if (p == NULL) return NULL;
		if (!StrEq(p, "{")) return NULL;

		// Read slave name
		p = strtok(NULL, "," BLANK_CHARACTERS);
		if (p == NULL) return NULL;
		slave = p;

		// Read data
		for (int i = 0; i < 5; i++)
		{
			p = strtok(NULL, "," BLANK_CHARACTERS);
			if (p == NULL) return NULL;
			data[i] = ParseInt(p);
		}

		// Read close bracket
		p = strtok(NULL, BLANK_CHARACTERS);
		if (p == NULL) return NULL;
		if (!StrEq(p, "}")) return NULL;
	}
	else if (StrEq(frame_name, "SaveConfiguration"))
	{
		type = LDF_SCMD_TYPE_SaveConfiguration;

		// Read open bracket
		p = strtok(NULL, BLANK_CHARACTERS);
		if (p == NULL) return NULL;
		if (!StrEq(p, "{")) return NULL;

		// Read slave name
		p = strtok(NULL, BLANK_CHARACTERS);
		if (p == NULL) return NULL;
		slave = p;

		// Read close bracket
		p = strtok(NULL, BLANK_CHARACTERS);
		if (p == NULL) return NULL;
		if (!StrEq(p, "}")) return NULL;
	}
	else if (StrEq(frame_name, "FreeFormat"))
	{
		type = LDF_SCMD_TYPE_FreeFormat;

		// Read open bracket
		p = strtok(NULL, BLANK_CHARACTERS);
		if (p == NULL) return NULL;
		if (!StrEq(p, "{")) return NULL;

		// Read data
		for (int i = 0; i < 8; i++)
		{
			p = strtok(NULL, "," BLANK_CHARACTERS);
			if (p == NULL) return NULL;
			data[i] = ParseInt(p);
		}

		// Read close bracket
		p = strtok(NULL, BLANK_CHARACTERS);
		if (p == NULL) return NULL;
		if (!StrEq(p, "}")) return NULL;
	}
	else if (StrEq(frame_name, "AssignFrameIdRange"))
	{
		type = LDF_SCMD_TYPE_AssignFrameIdRange;

		// Read open bracket
		p = strtok(NULL, BLANK_CHARACTERS);
		if (p == NULL) return NULL;
		if (!StrEq(p, "{")) return NULL;

		// Read slave name
		p = strtok(NULL, "," BLANK_CHARACTERS);
		if (p == NULL) return NULL;
		slave = p;

		// data[0] contains frame index in configurable_frames slave node list
		// Read frame index on configurable_frames slave list
		p = strtok(NULL, "," BLANK_CHARACTERS);
		if (p == NULL) return NULL;
		data[0] = ParseInt(p);

		// data[1] contains frame count
		// Count frames
		data[1] = 0;
		for (int i = 0; i < 4; i++)
		{
			p = strtok(NULL, "," BLANK_CHARACTERS);
			if (p == NULL) break;
			data[1]++;
		}

		// Check there is at least one frame pid
		if (data[1] == 0) return NULL;

		// Read close bracket
		p = strtok(NULL, BLANK_CHARACTERS);
		if (p == NULL) return NULL;
		if (!StrEq(p, "}")) return NULL;
	}
	else if (StrEq(frame_name, "AssignFrameId"))
	{
		type = LDF_SCMD_TYPE_AssignFrameId;

		// Read open bracket
		p = strtok(NULL, BLANK_CHARACTERS);
		if (p == NULL) return NULL;
		if (!StrEq(p, "{")) return NULL;

		// Read slave name
		p = strtok(NULL, "," BLANK_CHARACTERS);
		if (p == NULL) return NULL;
		slave = p;

		// Read frame name
		p = strtok(NULL, BLANK_CHARACTERS);
		if (p == NULL) return NULL;
		assign_frame = p;

		// Read close bracket
		p = strtok((char *) statement, BLANK_CHARACTERS);
		if (p == NULL) return NULL;
		if (!StrEq(p, "}")) return NULL;
	}
	else
	{
		type = LDF_SCMD_TYPE_UnconditionalFrame;
	}

	// Check delay word
	p = strtok(NULL, BLANK_CHARACTERS);
	if (p == NULL) return NULL;
	if (!StrEq(p, "delay")) return NULL;

	// Timeout
	p = strtok(NULL, BLANK_CHARACTERS);
	if (p == NULL) return NULL;
	timeout = atof(p);

	// Check ms word
	p = strtok(NULL, BLANK_CHARACTERS);
	if (p == NULL) return NULL;
	if (!StrEq(p, "ms")) return NULL;

	// Return command
	return new ldfschedulecommand(type, Str(frame_name), timeout, Str(slave), Str(data), Str(assign_frame));
}

ldfschedulecommand *ldfschedulecommand::FromUiScheduleListItemData(const uint8_t *command, const uint8_t *timeout)
{
	return NULL;
}

ldfschedulecommand::ldfschedulecommandtype_t ldfschedulecommand::GetType()
{
	return type;
}

uint8_t *ldfschedulecommand::GetFrameName()
{
	return frame_name;
}

uint16_t ldfschedulecommand::GetTimeoutMs()
{
	return timeout;
}

uint8_t *ldfschedulecommand::GetSlaveName()
{
	return slave_name;
}

uint8_t *ldfschedulecommand::GetData()
{
	return data;
}

uint8_t *ldfschedulecommand::GetAssignFrameName()
{
	return assign_frame_name;
}

void ldfschedulecommand::UpdateFrameName(const uint8_t *old_name, const uint8_t *new_name)
{
	if (StrEq(frame_name, old_name))
	{
		delete frame_name;
		frame_name = StrDup(new_name);
	}
}

void ldfschedulecommand::ValidateUnicity(uint8_t *schedule_table, ldfschedulecommand *command, uint8_t **validation_messages, uint32_t *validation_messages_count)
{
	char str[1000];

	if (StrEq(frame_name, command->frame_name))
	{
		sprintf(str, STR_ERR "Schedule table '%s' schedule command frame name '%s' repeated.", schedule_table, frame_name);
		validation_messages[*validation_messages_count++] = StrDup(str);
	}
}

const uint8_t * ldfschedulecommand::GetUiCommandText(ldf *db)
{
	static char res[1000];
	ldfnodeattributes *a;

	switch (type)
	{

	case LDF_SCMD_TYPE_MasterReq:
		strcpy(res, "MasterReq");
		break;

	case LDF_SCMD_TYPE_SlaveResp:
		strcpy(res, "SlaveResp");
		break;

	case LDF_SCMD_TYPE_AssignNAD:
		sprintf(res, "AssignNAD { %s }", slave_name);
		break;

	case LDF_SCMD_TYPE_DataDump:
		sprintf(res, "DataDump { %s, %02X, %02X, %02X, %02X, %02X }", slave_name, data[0], data[1], data[2], data[3], data[4]);
		break;

	case LDF_SCMD_TYPE_SaveConfiguration:
		sprintf(res, "SaveConfiguration { %s }", slave_name);
		break;

	case LDF_SCMD_TYPE_FreeFormat:
		sprintf(res, "FreeFormat { %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X }", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
		break;

	case LDF_SCMD_TYPE_AssignFrameIdRange:
		a = db->GetSlaveNodeAttributesByName(slave_name);
		sprintf(res, "AssignFrameIdRange { %s", slave_name);

		// data[0] is index, data[1] is count in configurable_frames
		for (int i = data[0]; i < data[1] && i < a->GetConfigurableFramesCount(); i++)
		{
			sprintf(res + strlen(res), ", %s", a->GetConfigurableFrame(i)->GetName());
		}
		strcat(res, " }");
		break;

	case LDF_SCMD_TYPE_AssignFrameId:
		sprintf(res, "AssignFrameId { %s, %s }", slave_name, assign_frame_name);
		break;

	default:
		strcpy(res, (char *)frame_name);
		break;

	}

	return Str(res);
}


} /* namespace lin */
