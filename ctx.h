/*
 * tcedit - line-oriented text editor.
 * Copyright (C) 2019  Thomas Cort <linuxgeek@gmail.com>
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

#ifndef TCE_CTX_H
#define TCE_CTX_H

#include <stdio.h>
#include <stdint.h>

#include "text.h"

struct context {
	FILE *in;
	FILE *out;
	size_t dot;
	int help_on;
	int prompt_on;
	char *prompt;
	int text_dirty;
	int done;
	struct text *text;
	char filename[FILENAME_MAX+1];
	int restricted;
	int suppress;
};

void ctx_free(struct context *ctx);

#endif
