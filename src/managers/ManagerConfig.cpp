/*
 * ManagerConfig.cpp
 *
 *  Created on: 22 jul. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include "ManagerConfig.h"


namespace managers {

ManagerConfig *ManagerConfig::manager = NULL;

ManagerConfig::ManagerConfig()
{
	database_path = NULL;

	struct passwd *pw = getpwuid(getuid());
	home_path = (uint8_t *)strdup(pw->pw_dir);
}

ManagerConfig::~ManagerConfig()
{
	if (home_path != NULL) delete home_path;
	if (database_path != NULL) delete database_path;
}

ManagerConfig *ManagerConfig::GetManager()
{
	if (manager == NULL)
		manager = new ManagerConfig();

	return manager;
}

uint8_t *ManagerConfig::GetDatabasePath()
{
	return database_path;
}

void ManagerConfig::Load()
{

}

void ManagerConfig::Store()
{

}


} /* namespace managers */
