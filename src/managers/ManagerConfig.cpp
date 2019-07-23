/*
 * ManagerConfig.cpp
 *
 *  Created on: 22 jul. 2019
 *      Author: iso9660
 */

#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include <stdio.h>
#include "ManagerConfig.h"


namespace managers {

ManagerConfig *ManagerConfig::manager = NULL;

ManagerConfig::ManagerConfig()
{
	database_path = NULL;

	struct passwd *pw = getpwuid(getuid());
	home_path = (uint8_t *)strdup(pw->pw_dir);

	Load();
}

ManagerConfig::~ManagerConfig()
{
	CleanParameters();
	if (home_path != NULL) delete home_path;
}

void ManagerConfig::CleanParameters()
{
	if (database_path != NULL)
		delete database_path;

	database_path = NULL;
}

ManagerConfig *ManagerConfig::GetManager()
{
	if (manager == NULL)
		manager = new ManagerConfig();

	return manager;
}

void ManagerConfig::Load()
{
	FILE *f = NULL;
	char str[10000];

	sprintf(str, "%s/.emulin", home_path);

	f = fopen(str, "r");
	if (f != NULL)
	{
		// Clean all parameters
		CleanParameters();

		// Read all new parameters
		while (fgets(str, 10000, f) != NULL)
		{
			char *p = strtok(str, " \t\r\n=");
			if (!p) continue;

			if (strcmp(p, "DATABASE") == 0)
			{
				p = strtok(NULL, "\"");
				if (p) database_path = (uint8_t *)strdup(p);
			}
		}

		fclose(f);
	}
}

void ManagerConfig::Store()
{
	FILE *f = NULL;
	char str[10000];

	sprintf(str, "%s/.emulin", home_path);

	f = fopen(str, "w");
	if (f != NULL)
	{
		fprintf(f, "DATABASE=\"%s\"\r\n", (database_path == NULL) ? "" : (char *)database_path);
		fclose(f);
	}
}

const uint8_t *ManagerConfig::GetDatabasePath()
{
	return database_path;
}

void ManagerConfig::SetDatabasePath(const uint8_t *path)
{
	if (database_path != NULL) delete database_path;
	database_path = (uint8_t *)strdup((char *) path);
}


} /* namespace managers */
