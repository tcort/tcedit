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

#include "re.h"

char *node_type_names[] = {
	"L_ANCHOR",
	"R_ANCHOR",
	"BASIC_REG_EXP"
};

node_t *mknode(node_type_t type, node_value_t value, node_t *nodes[]) {
	node_t *n;
	int i;

	fprintf(stdout, "DEBUG: FOUND %s\n", node_type_names[type]);

	n = (node_t *) malloc(sizeof(node_t));
	if (n == NULL) {
		return NULL;
	}
	memset(n, '\0', sizeof(node_t));

	n->type = type;
	n->value = value;
	for (i = 0; i < MAX_CHILD_NODES; i++) {
		n->nodes[i] = nodes[i];
	}
}

void freenode(node_t *n) {
	int i;

	if (n == NULL) {
		return;
	}

	for (i = 0; i < MAX_CHILD_NODES; i++) {
		if (n->nodes[i] != NULL) {
			freenode(n->nodes[i]);
		}
	}

	free(n);
	n = NULL;
}

node_t *parse_l_anchor(parser_t *p) {
	char ch;
	node_t *n;
	node_value_t val;
	node_t *nodes[MAX_CHILD_NODES];

	memset(nodes, '\0', sizeof(node_t *) * MAX_CHILD_NODES);

	ch = getc(p->f);
	if (ch == '^') {
		val.ch = ch;
		return mknode(L_ANCHOR, val, nodes);
	}
	ungetc(ch, p->f);
	return NULL;
}

node_t *parse_r_anchor(parser_t *p) {
	char ch, eof;
	node_t *n;
	node_value_t val;
	node_t *nodes[MAX_CHILD_NODES];

	memset(nodes, '\0', sizeof(node_t *) * MAX_CHILD_NODES);

	ch = getc(p->f);
	if (ch == '$' && p->length == ftell(p->f)) {
		val.ch = ch;
		return mknode(R_ANCHOR, val, nodes);
	}
	ungetc(ch, p->f);
	return NULL;
}

node_t *parse_basic_reg_exp(parser_t *p) {

	int i;
	node_t *n;
	node_t *nodes[MAX_CHILD_NODES];
	node_value_t value;
	char ch;

	memset(nodes, '\0', sizeof(node_t *) * MAX_CHILD_NODES);

	nodes[0] = parse_l_anchor(p);
	nodes[2] = parse_r_anchor(p);

	if (nodes[0] != NULL || nodes[1] != NULL || nodes[2] != NULL) {
		return mknode(BASIC_REG_EXP, value, nodes);
	}

	for (i = 0; i < MAX_CHILD_NODES; i++) {
		if (nodes[i] != NULL) {
			freenode(nodes[i]);
			nodes[i] = NULL;
		}
	}

	return NULL;
}

node_t *re_compile(char *pattern) {

	node_t *n;
	parser_t p;
	p.f = fmemopen(pattern, strlen(pattern), "r");

	fseek(p.f, 0, SEEK_END);
	p.length = ftell(p.f);
	rewind(p.f);

	n = parse_basic_reg_exp(&p);

	fclose(p.f);
	p.f = NULL;

	return n;
}
/*
int main(int argc, char *argv[]) {

	node_t *n;

	if (argc != 3) {
		fprintf(stderr, "usage: %s PATTERN SUBJECT\n", argv[0]);
		exit(1);
	}

	n = re_compile(argv[1]);
	fprintf(stdout, "%p\n", n);
	freenode(n);

	return 0;
}
*/