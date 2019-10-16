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

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "buffer.h"
#include "lines.h"

void ln_print(struct lines *text) {

	struct line *cur;

	for (cur = text->head; cur != NULL; cur = cur->tail) {
		printf("%s\n", cur->pos);
	}

}

void ln_append(struct lines *text, char *str) {

	struct line *ln;

	ln = (struct line *) malloc(sizeof(struct line));
	if (ln == NULL) {
		perror("?");
		exit(EXIT_FAILURE);
	}
	bzero(ln, sizeof(struct line));

	ln->pos = text->buf->buf + text->buf->cursize;
	bf_addstr(text->buf, str);
	bf_addch(text->buf, '\0');

	if (text->head == NULL && text->tail == NULL) {
		text->head = text->tail = ln;
	} else {
		ln->head = text->tail;
		text->tail->tail = ln;
		text->tail = ln;
	}
}

struct line *ln_getline(struct lines *text, size_t lineno) {

	int i;
	struct line *cur;

	if (text->head == NULL && text->tail == NULL) {
		return NULL;
	} else if (lineno == 1) {
		return text->head;
	}

	for (i = 1, cur = text->head; i < lineno && cur; i++, cur = cur->tail) {
		/* empty */;
	}

	return (i == lineno) ? cur : NULL;
}

size_t ln_count(struct lines *text) {

	size_t i;
	struct line *cur;

	for (i = 0, cur = text->head; cur; i++, cur = cur->tail) {
		/* empty */;
	}

	return i;

}
