/*
 * ldfmasternode.cpp
 *
 *  Created on: 3 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ldfcommon.h>
#include <ldfmasternode.h>


namespace lin {

ldfmasternode::ldfmasternode(const uint8_t *name, uint16_t timebase, uint16_t jitter) : ldfnode(name)
{
	this->timebase = timebase;
	this->jitter = jitter;
}

ldfmasternode::~ldfmasternode() {
}

ldfmasternode *ldfmasternode::FromLdfStatement(const uint8_t *statement)
{
	char *p = NULL;
	char *name = NULL;
	uint16_t timebase = 0;
	uint16_t jitter = 0;

	// Name
	p = strtok((char *)statement, "," BLANK_CHARACTERS);
	if (p) name = p;

	// Timebase
	if (p) p = strtok(NULL, "," BLANK_CHARACTERS);
	if (p) timebase = ParseInt(p);

	// Jitter
	if (p) p = strtok(NULL, "," BLANK_CHARACTERS);	// Skip word ms
	if (p) p = strtok(NULL, "," BLANK_CHARACTERS);
	if (p) jitter = atof(p) * 10;

	// Add master
	if (name != NULL)
	{
		return new ldfmasternode(Str(name), timebase, jitter);
	}
	else
	{
		return NULL;
	}
}

uint16_t ldfmasternode::GetTimebase()
{
	return timebase;
}

void ldfmasternode::SetTimebase(uint16_t timebase)
{
	this->timebase = timebase;
}

uint16_t ldfmasternode::GetJitter()
{
	return jitter;
}

void ldfmasternode::SetJitter(uint16_t jitter)
{
	this->jitter = jitter;
}

void ldfmasternode::ToLdfFile(FILE *f)
{
	fprintf(f, "Master: %s, %0.1f ms, %0.1f ms;\r\n", GetName(), 1.0f * timebase / 10.0f, 1.0f * jitter / 10.0f);
}




}
