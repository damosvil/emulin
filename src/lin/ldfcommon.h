/*
 * ldfcommon.h
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFCOMMON_H_
#define LIN_LDFCOMMON_H_

#include <stdint.h>


#define BLANK_CHARACTERS					" \t\r\n"

#define STR_ERR								"ERR: "
#define STR_WARN							"WARN: "


namespace lin
{

enum lin_protocol_version_e
{
	LIN_PROTOCOL_VERSION_NONE = 0,
	LIN_PROTOCOL_VERSION_2_0,
	LIN_PROTOCOL_VERSION_2_1
};

enum lin_language_version_e
{
	LIN_LANGUAGE_VERSION_NONE = 0,
	LIN_LANGUAGE_VERSION_2_0,
	LIN_LANGUAGE_VERSION_2_1
};

const char *GetLinProtocolVersionStringID(lin_protocol_version_e v);
const char *GetLinLanguageVersionStringID(lin_language_version_e v);
lin_protocol_version_e GetProtocolVersionByStringID(const char *id);
lin_language_version_e GetLanguageVersionByStringID(const char *id);
int32_t ParseInt(const char *p);
bool StrEq(const uint8_t *a, const uint8_t *b);
bool StrEq(const uint8_t *a, const char *b);
bool StrEq(const char *a, const uint8_t *b);
bool StrEq(const char *a, const char *b);
uint8_t *StrDup(const char *a);
uint8_t *StrDup(const uint8_t *a);
inline const uint8_t *Str(const char *c) { return (const uint8_t *)c; }


}

#endif /* LIN_LDFCOMMON_H_ */
