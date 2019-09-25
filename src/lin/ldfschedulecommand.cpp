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
	this->slave_name = StrDup(slave_name);
	for (uint32_t i = 0; i < 8 ; i++) this->data[i] = (data != NULL) ? data[i] : 0;
	this->assign_frame_name = StrDup(assign_frame_name);
}

ldfschedulecommand::~ldfschedulecommand()
{
	if (frame_name) delete frame_name;
	if (slave_name) delete slave_name;
	if (assign_frame_name) delete assign_frame_name;
}

char *ldfschedulecommand::ParseSlaveInBrackets(char *p, char **p_slave)
{
	p = StrTokenParseNextAndCheck(p, "{", BLANK_CHARACTERS);
	p = StrTokenParseNext(p, p_slave, BLANK_CHARACTERS);
	return StrTokenParseNextAndCheck(p, "}", BLANK_CHARACTERS);
}

char *ldfschedulecommand::ParseDataDump(char *p, char **p_slave, char *data)
{
	p = StrTokenParseNextAndCheck(p, "{", BLANK_CHARACTERS);
	p = StrTokenParseNext(p, p_slave, "," BLANK_CHARACTERS);	// Read slave
	for (int i = 0; (p != NULL) && (i < 5); i++)									// Read data dump
	{
		p = StrTokenParseNext(p, NULL, "," BLANK_CHARACTERS);
		if (p != NULL) data[i] = ParseInt(p);
	}
	return StrTokenParseNextAndCheck(p, "}", BLANK_CHARACTERS);
}

char *ldfschedulecommand::ParseFreeFormat(char *p, char *data)
{
	p = StrTokenParseNextAndCheck(p, "{", BLANK_CHARACTERS);
	for (int i = 0; (p != NULL) && (i < 8); i++)									// Read free format data
	{
		p = StrTokenParseNext(p, NULL, "," BLANK_CHARACTERS);
		data[i] = ParseInt(p);
	}
	return StrTokenParseNextAndCheck(p, "}", BLANK_CHARACTERS);
}

char *ldfschedulecommand::ParseAssignFrameId(char *p, char **p_slave, char **p_assign_frame)
{
	p = StrTokenParseNextAndCheck(p, "{", BLANK_CHARACTERS);
	p = StrTokenParseNext(p, p_slave, "," BLANK_CHARACTERS);		// Read slave
	p = StrTokenParseNext(p, p_assign_frame, "," BLANK_CHARACTERS);	// Read assign frame
	return StrTokenParseNextAndCheck(p, "}", BLANK_CHARACTERS);
}

char *ldfschedulecommand::ParseAssignFrameIdRange(char *p, char **p_slave, char **p_assign_frame, char *data)
{
	p = StrTokenParseNextAndCheck(p, "{", BLANK_CHARACTERS);
	p = StrTokenParseNext(p, p_slave, "," BLANK_CHARACTERS);	// Read slave

	// data[0] contains frame protected id in configurable_frames slave node list
	p = StrTokenParseNext(p, NULL, "," BLANK_CHARACTERS);
	data[0] = ParseInt(p);

	// Protected identifier cannot be 0, so return it as a string, and process it later
	if (data[0] == 0) *p_assign_frame = p;

	// data[1] contains frame count
	for (data[1] = 0; (p != NULL) && (data[1] < 4); data[1]++)
	{
		p = StrTokenParseNext(p, NULL, "," BLANK_CHARACTERS);
	}

	return StrTokenParseNextAndCheck(p, "}", BLANK_CHARACTERS);
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
	p = StrTokenParseFirst((char*)statement, &frame_name, BLANK_CHARACTERS);

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
		p = ParseSlaveInBrackets(p, &slave);
	}
	else if (StrEq(frame_name, "DataDump"))
	{
		type = LDF_SCMD_TYPE_DataDump;
		p = ParseDataDump(p, &slave, data);
	}
	else if (StrEq(frame_name, "SaveConfiguration"))
	{
		type = LDF_SCMD_TYPE_SaveConfiguration;
		p = ParseSlaveInBrackets(p, &slave);
	}
	else if (StrEq(frame_name, "FreeFormat"))
	{
		type = LDF_SCMD_TYPE_FreeFormat;
		p = ParseFreeFormat(p, data);
	}
	else if (StrEq(frame_name, "AssignFrameIdRange"))
	{
		type = LDF_SCMD_TYPE_AssignFrameIdRange;
		p = ParseAssignFrameIdRange(p, &slave, &assign_frame, data);
	}
	else if (StrEq(frame_name, "AssignFrameId"))
	{
		type = LDF_SCMD_TYPE_AssignFrameId;
		p = ParseAssignFrameId(p, &slave, &assign_frame);
	}
	else
	{
		type = LDF_SCMD_TYPE_UnconditionalFrame;
	}
	if (p == NULL) return NULL;

	// Check delay word
	p = StrTokenParseNextAndCheck(p, "delay", BLANK_CHARACTERS);
	if (p == NULL) return NULL;

	// Timeout
	p = StrTokenParseNext(p, NULL, BLANK_CHARACTERS);
	if (p == NULL) return NULL;
	timeout = ParseInt(p);

	// Check ms word
	p = StrTokenParseNextAndCheck(p, "ms", BLANK_CHARACTERS);
	if (p == NULL) return NULL;

	// Return command
	return new ldfschedulecommand(type, Str(frame_name), timeout, Str(slave), Str(data), Str(assign_frame));
}

ldfschedulecommand *ldfschedulecommand::FromStrCommand(ldf *db, const uint8_t *command, const uint8_t *timeout)
{
	ldfschedulecommand *c;
	char str[10000];

	// Emulate input from database
	sprintf(str, "%s delay %s ms", command, timeout);

	// Parse input
	c = FromLdfStatement(Str(str));
	if (c == NULL) return NULL;

	if (c->type == LDF_SCMD_TYPE_AssignFrameIdRange)
	{
		// Check that it couldn't parse the first frame as a PID, but as a frame name
		if (c->assign_frame_name == NULL) return NULL;

		// data[0] contains frame protected id in configurable_frames slave node list
		c->data[0] = db->GetFrameByName(c->assign_frame_name)->GetPid();
		delete c->assign_frame_name;
		c->assign_frame_name = NULL;

		// data[1] contains frame count, ie. it shall be bigger than 0 and smaller than 5
		if (c->data[1] == 0 || c->data[1] > 4) return NULL;
	}

	return c;
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

uint8_t *ldfschedulecommand::GetAssignFrameIdName()
{
	return assign_frame_name;
}

uint8_t ldfschedulecommand::GetAssignFrameIdRangePid()
{
	return data[0];
}

uint8_t ldfschedulecommand::GetAssignFrameIdRangeCount()
{
	return data[1];
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

const uint8_t *ldfschedulecommand::GetStrType()
{
	const char *res = NULL;

	switch (type)
	{

	case LDF_SCMD_TYPE_MasterReq:
		res = "MasterReq";
		break;

	case LDF_SCMD_TYPE_SlaveResp:
		res = "SlaveResp";
		break;

	case LDF_SCMD_TYPE_AssignNAD:
		res = "AssignNAD";
		break;

	case LDF_SCMD_TYPE_DataDump:
		res = "DataDump";
		break;

	case LDF_SCMD_TYPE_SaveConfiguration:
		res = "SaveConfiguration";
		break;

	case LDF_SCMD_TYPE_FreeFormat:
		res = "FreeFormat";
		break;

	case LDF_SCMD_TYPE_AssignFrameIdRange:
		res = "AssignFrameIdRange";
		break;

	case LDF_SCMD_TYPE_AssignFrameId:
		res = "AssignFrameId";
		break;

	default:
		res = "UnconditionalFrame";
		break;

	}

	return Str(res);
}

const uint8_t * ldfschedulecommand::GetStrCommand(ldf *db)
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

		// data[0] is first frame pid, data[1] is count in configurable_frames
		{
			ldfframe *ff = db->GetFrameByPid(data[0]);
			int cc = data[1];

			// Add frame names to command
			for (int i = 0; ff != NULL && i < a->GetConfigurableFramesCount(); i++)
			{
				if (StrEq(a->GetConfigurableFrame(i)->GetName(), ff->GetName()))
				{
					for (; i < a->GetConfigurableFramesCount() && cc > 0; i++)
					{
						sprintf(res + strlen(res), ", %s", a->GetConfigurableFrame(i)->GetName());
						cc--;
					}
				}
			}
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

const uint8_t *ldfschedulecommand::GetStrRawData()
{
	static char res[1000];

	switch (type)
	{

	case LDF_SCMD_TYPE_DataDump:
		sprintf(res, "%02X, %02X, %02X, %02X, %02X", data[0], data[1], data[2], data[3], data[4]);
		break;

	case LDF_SCMD_TYPE_FreeFormat:
		sprintf(res, "%02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
		break;

	default:
		strcpy(res, "");
		break;

	}

	return Str(res);
}



} /* namespace lin */
