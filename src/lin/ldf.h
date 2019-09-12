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

	static int SorterSignals(const void *a, const void *b);
	static int SorterFrames(const void *a, const void *b);

	void DeleteSlaveNodeByIndex(uint32_t ix);
	void DeleteSlaveNodeAttributesByIndex(uint32_t ix);
	void DeleteSignalByIndex(uint32_t ix);
	void DeleteFrameByIndex(uint32_t ix);
	void DeleteScheduleTableByIndex(uint32_t ix);

public:
	ldf(const uint8_t *filename);
	virtual ~ldf();

	void SortData();
	bool Validate(void);

	lin_protocol_version_e GetLinProtocolVersion();
	void SetLinProtocolVersion(lin_protocol_version_e v);

	lin_language_version_e GetLinLanguageVersion();
	void SetLinLanguageVersion(lin_language_version_e v);

	uint16_t GetLinSpeed();
	void SetLinSpeed(uint16_t s);

	ldfmasternode *GetMasterNode();
	ldfnode *GetSlaveNodeByIndex(uint32_t ix);
	uint32_t GetSlaveNodesCount();

	ldfnodeattributes *GetSlaveNodeAttributesByName(const uint8_t *slave_name);
	void AddSlaveNode(ldfnodeattributes *n);
	void UpdateSlaveNode(const uint8_t *old_slave_name, ldfnodeattributes *n);
	void DeleteSlaveNode(const uint8_t *slave_name);

	ldfsignal *GetSignalByIndex(uint32_t ix);
	ldfsignal *GetSignalByName(const uint8_t *signal_name);
	uint32_t GetSignalsCount();
	void AddSignal(ldfsignal *s);
	void UpdateSignal(const uint8_t *old_signal_name, ldfsignal *s);
	void DeleteSignal(const uint8_t *signal_name);

	void UpdateMasterNodeName(const uint8_t *old_name, const uint8_t *new_name);

	ldfframe *GetFrameByIndex(uint32_t ix);
	ldfframe *GetFrameByName(const uint8_t *frame_name);
	ldfframe *GetFrameById(uint8_t frame_id);
	uint32_t GetFramesCount();
	void AddFrame(ldfframe *f);
	void UpdateFrame(const uint8_t *old_frame_name, ldfframe *f);
	void DeleteFrame(const uint8_t *frame_name);

	ldfscheduletable *GetScheduleTableByIndex(uint32_t ix);
	ldfscheduletable *GetScheduleTableByName(const uint8_t *name);
	uint32_t GetScheduleTablesCount();
	void AddScheduleTable(ldfscheduletable *t);
	void UpdateScheduleTable(const uint8_t *old_schedule_table_name, ldfscheduletable *t);
	void DeleteScheduleTable(const uint8_t *schedule_table_name);

};

} /* namespace lin */

#endif /* LIN_LDF_H_ */
