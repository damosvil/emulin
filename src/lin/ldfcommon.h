/*
 * ldfcommon.h
 *
 *  Created on: 5 jul. 2019
 *      Author: iso9660
 */

#ifndef LIN_LDFCOMMON_H_
#define LIN_LDFCOMMON_H_

#define BLANK_CHARACTERS					" \t\r\n"

#define STR_ERR								"ERR: "
#define STR_WARN							"WARN: "

namespace lin
{

enum lin_protocol_version_e
{
	LIN_PROTOCOL_VERSION_NONE,
	LIN_PROTOCOL_VERSION_2_0,
	LIN_PROTOCOL_VERSION_2_1
};


}


#endif /* LIN_LDFCOMMON_H_ */
