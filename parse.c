/*
 * tcedit - a line oriented text editor.
 * Copyright (C) 2018, 2019  Thomas Cort <linuxgeek@gmail.com>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"
#include "match.h"

struct input parse(char *command) {
	int i;
	int rc;
	char *result[NMATCHES];
	int nmatches;
	struct input in;

	bzero(&in, sizeof(struct input));

	nmatches = match(command, "([0-9]*)?,?([0-9]*)?([A-Za-z\\=])(.*)", result);
	if (nmatches == 5) {
		in.start = atoi(result[1]);
		in.end = atoi(result[2]);
		in.letter = result[3][0];
	}

	for (i = 0; nmatches > 0 && i < nmatches; i++) {
		free(result[i]);
	}

	return in;
}
