/*
 * ldfnodeattributes.cpp
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <stdlib.h>
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
	if (name) delete name;
	if (response_error_signal_name) delete response_error_signal_name;
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

} /* namespace lin */
