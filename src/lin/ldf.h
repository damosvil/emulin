/*
 * ldf.h
 *
 *  Created on: 3 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDF_H_
#define LIN_LDF_H_

#include <stdint.h>
#include <ldfcommon.h>
#include <ldfmasternode.h>
#include <ldfsignal.h>
#include <ldfframe.h>
#include <ldfframesignal.h>
#include <ldfnodeattributes.h>
#include <ldfscheduletable.h>
#include <ldfschedulecommand.h>
#include <ldfencodingsignals.h>
#include <ldfencodingtype.h>


namespace lin {

class ldf {

private:
	enum ldf_parsing_state_e
	{
		LDF_PARSING_STATE_NONE,
		LDF_PARSING_STATE_NODES,
		LDF_PARSING_STATE_SIGNALS,
		LDF_PARSING_STATE_FRAMES,
		LDF_PARSING_STATE_NODES_ATTRIBUTES,
		LDF_PARSING_STATE_CONFIGURABLE_FRAMES,
		LDF_PARSING_STATE_SCHEDULE_TABLES,
		LDF_PARSING_STATE_ENCODING_TYPES,
		LDF_PARSING_STATE_ENCODING_SIGNALS
	};

private:
	// State
	bool is_lin_description_file;
	ldf_parsing_state_e parsing_state;
	uint32_t group_level;

	// Global parameters
	lin_protocol_version_e lin_protocol_version;
	lin_language_version_e lin_language_version;
	uint32_t lin_speed;

	// Nodes
	ldfmasternode *master;
	ldfnode *slaves[128];
	uint8_t slaves_count;

	// Signals
	ldfsignal *signals[10000];
	uint32_t signals_count;

	// Frames
	ldfframe *frames[1000];
	uint32_t frames_count;

	// Node attributes
	ldfnodeattributes *node_attributes[1000];
	uint32_t node_attributes_count;

	// Schedule tables
	ldfscheduletable *schedule_tables[100];
	uint32_t schedule_tables_count;

	// Signal encodings
	ldfencodingtype *encoding_types[1000];
	uint32_t encoding_types_count;

	// Relationships between encodings and signals
	ldfencodingsignals *encoding_signals[1000];
	uint32_t encoding_signals_count;

	// Validation messages
	uint8_t *validation_messages[10000];
	uint32_t validation_messages_count;


private:
	bool char_in_set(uint8_t c, const char *set);
	void parse_char(uint8_t c);
	void process_statement(uint8_t *statement);
	void process_group_start(uint8_t *start);
	void process_group_end(uint8_t *end);

public:
	ldf(const uint8_t *filename);
	virtual ~ldf();

	bool Validate(void);

	lin_protocol_version_e GetLinProtocolVersion();
	void SetLinProtocolVersion(lin_protocol_version_e v);

	lin_language_version_e GetLinLanguageVersion();
	void SetLinLanguageVersion(lin_language_version_e v);

	uint16_t GetLinSpeed();
	void SetLinSpeed(uint16_t s);


};

} /* namespace lin */

#endif /* LIN_LDF_H_ */
