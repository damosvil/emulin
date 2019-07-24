/*
 * tools.c
 *
 *  Created on: 24 jul. 2019
 *      Author: iso9660
 */

#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "tools.h"


uint32_t GetStrIndexByID(const char **ids, uint32_t ids_count, const char *id)
{
	uint32_t ix = 0;

	for (ix = 0; ix < ids_count; ix++)
	{
		if (id == NULL) break;
		if (ids[ix] == NULL) continue;
		if (strcmp(id, ids[ix]) == 0) break;
	}

	return ix;
}

gboolean KeyFilterNumbers(GtkWidget *widget, GdkEvent  *event, gpointer   user_data)
{
	const char *valid = "0123456789\b";

	if (event->type == GDK_KEY_PRESS)
	{
		while (*valid)
		{
			if (event->key.string[0] == *valid) return 0;
			valid++;
		}
	}

	return 1;
}
