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

#ifndef __TCE_LINES_H
#define __TCE_LINES_H

#include "buffer.h"

struct line {
	char *pos;
	struct line *head;
	struct line *tail;
};

struct lines {
	struct line *head;
	struct line *tail;
	struct buffer *buf;
};

int ln_append(struct lines *text, char *str);
void ln_print(struct lines *text);
struct line *ln_getline(struct lines *text, size_t lineno);
size_t ln_count(struct lines *text);
void ln_free(struct lines *text);

#endif
