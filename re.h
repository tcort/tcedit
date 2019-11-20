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

#ifndef __TCE_RE_H
#define __TCE_RE_H

#define MAX_CHILD_NODES (8)

extern char *node_type_names[];

enum node_type {
	L_ANCHOR = 0,
	R_ANCHOR = 1,
	BASIC_REG_EXP = 2
};

union node_value {
	char ch;
	int backref;
};

struct node {
	enum node_type type;
	union node_value value;
	struct node *nodes[MAX_CHILD_NODES];
};

struct parser {
	FILE *f;
	long length;
};

typedef union node_value node_value_t;
typedef enum node_type node_type_t;
typedef struct node node_t;
typedef struct parser parser_t;

node_t *re_compile(char *pattern);
void re_free(node_t *n);

#endif
