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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ctx.h"
#include "parse.h"
#include "text.h"



struct input parse(struct context *ctx, char *line) {
	struct input in;
	char *p;

	memset(&in, '\0', sizeof(struct input));

	p = line;

	in.line1 = (unsigned long) atoi(p);
	while (p != NULL && isdigit(*p)) {
		++p;
	}

	if (*p == ',') {
		in.comma = 1;
		++p;
	}

	in.line2 = (unsigned long) atoi(p);
	while (p != NULL && isdigit(*p)) {
		++p;
	}

	in.letter = *p;
	++p;
	while (p != NULL && *p == ' ') {
		++p;
	}
	in.params = strdup(p);
	for (p = in.params; *p != '\0'; p++) {
		if (*p == '\n') {
			*p = '\0';
		}
	}


	if (in.line2 == 0 && in.line1 == 0 && in.comma == 1) {
		in.line1 = 1;
		in.line2 = text_count(ctx->text);
	} else if (in.line2 == 0 && in.line1 != 0) {
		in.line2 = in.line1;
	}

	return in;
}
