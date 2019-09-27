/*
 * ldfencodingsignals.cpp
 *
 *  Created on: 16 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ldfcommon.h>
#include <ldfencodingsignals.h>


namespace lin {

ldfencodingsignals::ldfencodingsignals(const uint8_t *encoding_name)
{
	this->encoding_name = StrDup(encoding_name);
	this->signals_count = 0;
}

ldfencodingsignals::~ldfencodingsignals()
{
	if (encoding_name) delete encoding_name;
	while (signals_count > 0) delete signals[--signals_count];
}

void ldfencodingsignals::AddSignal(const uint8_t *signal)
{
	signals[signals_count++] = StrDup(signal);
}

ldfencodingsignals *ldfencodingsignals::FromLdfStatement(const uint8_t *statement)
{
	char *encoding_name;
	ldfencodingsignals *s;

	// Type name
	char *p = strtok((char *)statement, ":," BLANK_CHARACTERS);
	if (!p) return NULL;
	encoding_name = p;

	// First signal
	p = strtok(NULL, "," BLANK_CHARACTERS);
	if (!p) return NULL;

	// Create relationship between encoding and signal
	s = new ldfencodingsignals(Str(encoding_name));

	// If any add more signals
	while (p)
	{
		s->AddSignal(Str(p));
		p = strtok(NULL, "," BLANK_CHARACTERS);
	}

	return s;
}

void ldfencodingsignals::ValidateUnicity(ldfencodingsignals *encoding, uint8_t **validation_messages, uint32_t *validation_messages_count)
{
	char str[1000];

	if (StrEq(encoding_name, encoding->encoding_name))
	{
		sprintf(str, STR_ERR "Encoding name '%s' repeated.", encoding_name);
		validation_messages[*validation_messages_count++] = StrDup(str);
	}
}

void ldfencodingsignals::ValidateSignals(ldfsignal **signals, uint32_t signals_count, uint8_t **validation_messages, uint32_t *validation_messages_count)
{
	uint32_t i, j;
	char str[1000];

	for (i = 0; i < this->signals_count; i++)
	{
		ldfsignal *s = NULL;

		// Look for signal
		for (j = 0; (s == NULL) && (j < signals_count); j++)
		{
			s = StrEq(signals[j]->GetName(), this->signals[i]) ? signals[j] : NULL;
		}

		// Check signal is defined
		if (s == NULL)
		{
			sprintf(str, STR_ERR "Signal representation '%s' uses signal '%s' not defined.", encoding_name, this->signals[i]);
			validation_messages[*validation_messages_count++] = StrDup(str);
		}
	}
}


} /* namespace lin */
