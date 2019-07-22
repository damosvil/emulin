/*
 * ManagerConfig.h
 *
 *  Created on: 22 jul. 2019
 *      Author: iso9660
 */

#ifndef MANAGERS_MANAGERCONFIG_H_
#define MANAGERS_MANAGERCONFIG_H_


#include <stdint.h>


namespace managers {

class ManagerConfig {

private:
	uint8_t *home_path;
	uint8_t *database_path;

	ManagerConfig();
	virtual ~ManagerConfig();

	static ManagerConfig *manager;

public:
	static ManagerConfig *GetManager();

	uint8_t *GetDatabasePath();
	void Load();
	void Store();

};

} /* namespace managers */

#endif /* MANAGERS_MANAGERCONFIG_H_ */
