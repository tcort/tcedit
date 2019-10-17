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

#ifndef __TCE_BUFFER_H
#define __TCE_BUFFER_H

#include <stdlib.h>

struct buffer {
	char *buf;      /* pointer returned by malloc */
	size_t maxsize;    /* size of space allocated by malloc */
	size_t cursize;    /* amount of space currently used */
	size_t incr;       /* increment to add when growing maxsize */
};

struct buffer *bf_alloc(size_t size, size_t incr);
void bf_addch(struct buffer *buf, char ch);
void bf_addstr(struct buffer *buf, char *str);
void bf_clear(struct buffer *buf);
void bf_free(struct buffer *buf);

#endif
