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

#ifndef __TCE_TEXT_H
#define __TCE_TEXT_H

struct text {
	int scratchfd;
	char tmpfile[32];
	FILE *scratch;

	long *lines;
	size_t incr;
	size_t capacity;
	size_t length;

	size_t bytes; /* only valid after read/write call and only reflects bytes read/written */

	char *line;
	size_t cap;
	ssize_t len;
};

struct text *text_new(void);
size_t text_count(struct text *t);
size_t text_delete(struct text *t, size_t begin, size_t end);
void text_extend(struct text *t);
int text_appendln(struct text *t, char *s, size_t where);
int text_append(struct text *t, struct text *tin, size_t where);
struct text *text_read(FILE *in, int period_ends_input);
void text_write(struct text *t, FILE *out, size_t begin, size_t end, int show_lineno);
char *text_getln(struct text *t, size_t lineno);
int text_putln(struct text *t, size_t lineno, char *s);
void text_free(struct text *t);

#endif
