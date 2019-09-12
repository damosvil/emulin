/*
 * ldfconfigurableframe.h
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFCONFIGURABLEFRAME_H_
#define LIN_LDFCONFIGURABLEFRAME_H_

#include <stdint.h>

namespace lin {

class ldfconfigurableframe {

private:
	uint8_t *name;
	uint8_t id;

public:
	ldfconfigurableframe(const uint8_t *name, uint8_t id);
	virtual ~ldfconfigurableframe();

	static ldfconfigurableframe *FromLdfStatement(uint8_t *statement);
	static int32_t SorterConfigurableFrames(const void *a, const void *b);

	uint8_t *GetName();
	uint8_t GetId();

	void ValidateUnicity(uint8_t *attributes, ldfconfigurableframe *frame, uint8_t **validation_messages, uint32_t *validation_messages_count);
	void UpdateName(const uint8_t *old_frame_name, const uint8_t *new_frame_name);

};

} /* namespace lin */

#endif /* LIN_LDFCONFIGURABLEFRAME_H_ */
