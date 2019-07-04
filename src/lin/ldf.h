/*
 * ldf.h
 *
 *  Created on: 3 jul. 2019
 *      Author: ctag
 */

#ifndef LIN_LDF_H_
#define LIN_LDF_H_

#include <stdint.h>
#include <ldfmasternode.h>

namespace lin {

class ldf {

private:
	enum lin_protocol_version_e
	{
		LIN_PROTOCOL_VERSION_NONE,
		LIN_PROTOCOL_VERSION_2_1
	};

	enum lin_language_version_e
	{
		LIN_LANGUAGE_VERSION_NONE,
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

private:
	// State
	bool is_lin_description_file;
	ldf_parsing_state_e parsing_state;
	uint32_t bracket_level;

	// Global parameters
	lin_protocol_version_e lin_protocol_version;
	lin_language_version_e lin_language_version;
	uint32_t lin_speed;

	// Nodes
	ldfmasternode *master;
	ldfnode *slaves[128];
	uint8_t slaves_count;

private:
	void parse_state_none(char *token);
	void parse_state_nodes(char *token);
	void parse_state_signals(char *token);

};

} /* namespace lin */

#endif /* LIN_LDF_H_ */
