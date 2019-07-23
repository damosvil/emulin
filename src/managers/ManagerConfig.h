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

private:
	ManagerConfig();
	virtual ~ManagerConfig();

	void CleanParameters();

	static ManagerConfig *manager;

public:
	static ManagerConfig *GetManager();

	void Load();
	void Store();

	const uint8_t *GetDatabasePath();
	void SetDatabasePath(const uint8_t *path);

};

} /* namespace managers */

#endif /* MANAGERS_MANAGERCONFIG_H_ */
