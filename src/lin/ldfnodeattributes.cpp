/*
 * ldfnodeattributes.cpp
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ldfcommon.h>
#include <ldfnodeattributes.h>

namespace lin {

ldfnodeattributes::ldfnodeattributes(uint8_t *name)
{
	this->name = (uint8_t *)strdup((char *)name);
	this->protocol = LIN_PROTOCOL_VERSION_NONE;
	this->configured_NAD = 0xFF;
	this->initial_NAD = 0xFF;
	this->product_id = { 0, 0, 0 };
	this->response_error_signal_name = NULL;
	this->fault_state_signals_count = 0;
	this->P2_min = 50;
	this->ST_min = 0;
	this->N_As_timeout = 1000;
	this->N_Cr_timeout = 1000;
	this->configurable_frames_count = 0;
}

ldfnodeattributes::~ldfnodeattributes()
{
	if (name != NULL) delete name;
	if (response_error_signal_name != NULL) delete response_error_signal_name;
	while (fault_state_signals_count > 0) delete fault_state_signals[--fault_state_signals_count];
	while (configurable_frames_count > 0) delete configurable_frames[--configurable_frames_count];
}

void ldfnodeattributes::UpdateFromLdfStatement(uint8_t *statement)
{
	char *p = NULL;

	// Isolate parameter name
	p = strtok((char *) statement, "=," BLANK_CHARACTERS);
	if (!p) return;

	// Isolate and parse parameter value
	if (strcmp(p, "LIN_protocol") == 0)
	{
		p = strtok(NULL, "=," BLANK_CHARACTERS);
		if (!p) return;

		if (strcmp(p, "\"2.1\"") == 0)
			protocol = LIN_PROTOCOL_VERSION_2_1;
		else if (strcmp(p, "\"2.0\"") == 0)
			protocol = LIN_PROTOCOL_VERSION_2_0;
	}
	else if (strcmp(p, "configured_NAD") == 0)
	{
		p = strtok(NULL, "=," BLANK_CHARACTERS);
		if (p) configured_NAD = (p[1] == 'x' || p[1] == 'X') ? strtoul(p, NULL, 16) : atoi(p);
	}
	else if (strcmp(p, "initial_NAD") == 0)
	{
		p = strtok(NULL, "=," BLANK_CHARACTERS);
		if (p) initial_NAD = (p[1] == 'x' || p[1] == 'X') ? strtoul(p, NULL, 16) : atoi(p);
	}
	else if (strcmp(p, "product_id") == 0)
	{
		// Supplier ID
		p = strtok(NULL, "=," BLANK_CHARACTERS);
		if (p) product_id.supplier_id = (p[1] == 'x' || p[1] == 'X') ? strtoul(p, NULL, 16) : atoi(p);

		// Function ID
		if (p) p = strtok(NULL, "=," BLANK_CHARACTERS);
		if (p) product_id.function_id = (p[1] == 'x' || p[1] == 'X') ? strtoul(p, NULL, 16) : atoi(p);

		// Variant
		if (p) p = strtok(NULL, "=," BLANK_CHARACTERS);
		if (p) product_id.variant = (p[1] == 'x' || p[1] == 'X') ? strtoul(p, NULL, 16) : atoi(p);
	}
	else if (strcmp(p, "response_error") == 0)
	{
		p = strtok(NULL, "=,");
		if (p) response_error_signal_name = (uint8_t *)strdup(p);
	}
	else if (strcmp(p, "fault_state_signals") == 0)
	{
		while (p)
		{
			p = strtok(NULL, "=,");
			if (p) fault_state_signals[fault_state_signals_count++] = (uint8_t *)strdup(p);
		}
	}
	else if (strcmp(p, "P2_min") == 0)
	{
		p = strtok(NULL, "=,");
		if (p) P2_min = atof(p) * 10;
	}
	else if (strcmp(p, "ST_min") == 0)
	{
		p = strtok(NULL, "=,");
		if (p) ST_min = atof(p) * 10;
	}
	else if (strcmp(p, "N_As_timeout") == 0)
	{
		p = strtok(NULL, "=,");
		if (p) N_As_timeout = atof(p) * 10;
	}
	else if (strcmp(p, "N_Cr_timeout") == 0)
	{
		p = strtok(NULL, "=,");
		if (p) N_Cr_timeout = atof(p) * 10;
	}
}

void ldfnodeattributes::AddConfigurableFrame(ldfconfigurableframe *frame)
{
	configurable_frames[configurable_frames_count++] = frame;
}

void ldfnodeattributes::ValidateNode(ldfnode **slaves, uint32_t slaves_count, uint8_t **validation_messages, uint32_t *validation_messages_count)
{
	char str[1000];

	if (this->protocol == LIN_PROTOCOL_VERSION_NONE)
	{
		sprintf(str, STR_ERR "Node_attributes '%s' protocol not defined.", name);
		validation_messages[*validation_messages_count++] = (uint8_t *)strdup(str);
	}

	if (!ldfnode::CheckNodeName(name, NULL, slaves, slaves_count))
	{
		sprintf(str, STR_ERR "Node_attributes '%s' node not defined in database's slaves", name);
		validation_messages[*validation_messages_count++] = (uint8_t *)strdup(str);
	}
}

void ldfnodeattributes::ValidateUnicity(ldfnodeattributes *attributes, uint8_t **validation_messages, uint32_t *validation_messages_count)
{
	char str[1000];

	if (strcmp((char *)name, (char *)attributes->name) == 0)
	{
		sprintf(str, STR_ERR "Node_attributes '%s' node defined twice", name);
		validation_messages[*validation_messages_count++] = (uint8_t *)strdup(str);
	}
}

void ldfnodeattributes::ValidateFrames(ldfframe **frames, uint32_t frames_count, uint8_t **validation_messages, uint32_t *validation_messages_count)
{
	char str[1000];
	uint32_t i, j;

	for (i = 0; i < configurable_frames_count; i++)
	{
		ldfframe *f = NULL;

		// Look for frame definition
		for (j = 0; (f == NULL) && (j < frames_count); j++)
		{
			f = (strcmp((char *)frames[j]->GetName(), (char *)configurable_frames[i]->GetName()) == 0) ? frames[j] : NULL;
		}

		// Check frame exists
		if (f == NULL)
		{
			sprintf(str, STR_ERR "Node_attributes '%s' configurable frame '%s' not defined.", name, configurable_frames[i]->GetName());
			validation_messages[*validation_messages_count++] = (uint8_t *)strdup(str);
			continue;
		}

		// Check configurable frame repeated
		for (j = i + 1; j < configurable_frames_count; j++)
		{
			configurable_frames[i]->ValidateUnicity(name, configurable_frames[j], validation_messages, validation_messages_count);
		}
	}
}

uint8_t *ldfnodeattributes::GetName()
{
	return name;
}

lin_protocol_version_e ldfnodeattributes::GetProtocolVersion()
{
	return protocol;
}

uint8_t ldfnodeattributes::GetInitialNAD()
{
	return initial_NAD;
}

uint8_t ldfnodeattributes::GetConfiguredNAD()
{
	return configured_NAD;
}

uint16_t ldfnodeattributes::GetSupplierID()
{
	return product_id.supplier_id;
}

uint16_t ldfnodeattributes::GetFunctionID()
{
	return product_id.function_id;
}

uint8_t ldfnodeattributes::GetVariant()
{
	return product_id.variant;
}

uint8_t *ldfnodeattributes::GetResponseErrorSignalName()
{
	return response_error_signal_name;
}

ldfconfigurableframe *ldfnodeattributes::GetConfigurableFrame(uint32_t ix)
{
	return configurable_frames[ix];
}

uint16_t ldfnodeattributes::GetConfigurableFramesCount()
{
	return configurable_frames_count;
}

uint16_t ldfnodeattributes::GetP2_min()
{
	return P2_min;
}

uint16_t ldfnodeattributes::GetST_min()
{
	return ST_min;
}

uint16_t ldfnodeattributes::GetN_As_timeout()
{
	return N_As_timeout;
}

uint16_t ldfnodeattributes::GetN_Cr_timeout()
{
	return N_Cr_timeout;
}

void ldfnodeattributes::SetProtocolVersion(lin_protocol_version_e v)
{
	protocol = v;
}

void ldfnodeattributes::SetInitialNAD(uint8_t v)
{
	initial_NAD = v;
}

void ldfnodeattributes::SetConfiguredNAD(uint8_t v)
{
	configured_NAD = v;
}

void ldfnodeattributes::SetSupplierID(uint16_t v)
{
	product_id.supplier_id = v;
}

void ldfnodeattributes::SetFunctionID(uint16_t v)
{
	product_id.function_id = v;
}

void ldfnodeattributes::SetVariant(uint8_t v)
{
	product_id.variant = v;
}

void ldfnodeattributes::SetP2_min(uint16_t v)
{
	P2_min = v;
}

void ldfnodeattributes::SetST_min(uint16_t v)
{
	ST_min = v;
}

void ldfnodeattributes::SetN_As_timeout(uint16_t v)
{
	N_As_timeout = v;
}

void ldfnodeattributes::SetN_Cr_timeout(uint16_t v)
{
	N_Cr_timeout = v;
}

void ldfnodeattributes::SetResponseErrorSignalName(uint8_t *v)
{
	if (response_error_signal_name != NULL) delete response_error_signal_name;
	response_error_signal_name = (v != NULL) ? (uint8_t *)strdup((const char *)v) : NULL;
}

bool ldfnodeattributes::NameIs(uint8_t *name)
{
	return strcmp((char *)name, (char *)this->name) == 0;
}

void ldfnodeattributes::SortData()
{
	qsort(configurable_frames, configurable_frames_count, sizeof(configurable_frames[0]), ldfconfigurableframe::SorterConfigurableFrames);
}

void ldfnodeattributes::UpdateConfigurableFrames(const uint8_t *old_frame_name, const uint8_t *new_frame_name)
{
	for (int i = 0; i < configurable_frames_count; i++)
	{
		configurable_frames[i]->UpdateName(old_frame_name, new_frame_name);
	}
}

void ldfnodeattributes::DeleteConfigurableFrames(const uint8_t *frame_name)
{
	for (int i = 0; i < configurable_frames_count; i++)
	{
		// Skip configurable frames with different name
		if (strcmp((char *)configurable_frames[i], (char* )frame_name) != 0)
		{
			continue;
		}

		// Delete configurable frame and move back the other frames over the gap
		delete configurable_frames[i];
		configurable_frames_count--;
		for (; i < configurable_frames_count; i++)
			configurable_frames[i] = configurable_frames[i + 1];
		break;
	}
}


} /* namespace lin */
