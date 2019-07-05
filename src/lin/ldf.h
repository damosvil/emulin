/*
 * ldf.h
 *
 *  Created on: 3 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDF_H_
#define LIN_LDF_H_

#include <stdint.h>
#include <ldfmasternode.h>
#include <ldfsignal.h>
#include <ldfframe.h>
#include <ldfframesignal.h>


namespace lin {

class ldf {

private:
	enum lin_protocol_version_e
	{
		LIN_PROTOCOL_VERSION_NONE,
		LIN_PROTOCOL_VERSION_2_0,
		LIN_PROTOCOL_VERSION_2_1
	};

	enum lin_language_version_e
	{
		LIN_LANGUAGE_VERSION_NONE,
		LIN_LANGUAGE_VERSION_2_0,
		LIN_LANGUAGE_VERSION_2_1
	};

	enum ldf_parsing_state_e
	{
		LDF_PARSING_STATE_NONE,
		LDF_PARSING_STATE_NODES,
		LDF_PARSING_STATE_SIGNALS,
		LDF_PARSING_STATE_FRAMES,
		LDF_PARSING_STATE_NODES_ATTRIBUTES,
	};

public:
	ldf(uint8_t *filename);
	virtual ~ldf();

	void Validate(void);


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


private:
	bool char_in_set(uint8_t c, const char *set);
	void parse_char(uint8_t c);
	void process_statement(uint8_t *statement);
	void process_group_start(uint8_t *start);
	void process_group_end(uint8_t *end);

};

} /* namespace lin */

#endif /* LIN_LDF_H_ */
