/*
 * tcedit - line-oriented text editor.
 * Copyright (C) 2019, 2020  Thomas Cort <linuxgeek@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <string.h>
#include <stdlib.h>

#include "dstring.h"

struct dstring dstring_new(void) {
	struct dstring ds;
	ds.s = NULL;
	ds.len = 0;
	return ds;
}

void dstring_append(struct dstring *ds, char *s, size_t len) {
	char *newstr;

	if (ds->len + len < ds->len) {
		return; /* integer overflow */
	}

	newstr = (char *) realloc(ds->s, ds->len + len);
	if (newstr == NULL) {
		return;
	}

	memcpy(&newstr[ds->len], s, len);
	ds->s = newstr;
	ds->len += len;
}

void dstring_free(struct dstring *ds) {
	if (ds->s != NULL) {
		free(ds->s);
		ds->s = NULL;
		ds->len = 0;
	}
}
