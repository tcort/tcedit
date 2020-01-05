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

#ifndef TCE_PARSE_H
#define TCE_PARSE_H

#include <stddef.h>

#include "ctx.h"

struct input {
	size_t line1;
	size_t line2;
	size_t nlines;
	int comma;
	char letter;
	char *params;
};

struct input parse(struct context *ctx, char *line);

#endif
