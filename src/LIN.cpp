/*
 ============================================================================
 Name        : LIN.cpp
 Author      : ISO9660
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C++,
 ============================================================================
 */

#include <iostream>
#include <stdexcept>
#include "ldf.h"

using namespace std;

#define FILENAME				"config/database.ldf"

int main(void) {
	lin::ldf *db = NULL;

	// Instantiate database
	try
	{
		db = new lin::ldf((uint8_t *)FILENAME);
	}
	catch (runtime_error &e)
	{
		cout << e.what() << endl;
		cout << "Error parsing " << FILENAME << endl; /* prints Hello World */
	}

	// Resource erasing
	if (db) delete db;

	return 0;
}
