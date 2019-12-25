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

#include "config.h"

#include <limits.h>
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tcre.h"

enum tcre_type { ORD='c', CCL='[', LANCHOR='^', RANCHOR='$', DOT='.', STAR='*', PLUS='+', QUEST='?' };
typedef enum tcre_type tcre_type_t;
struct tcre {
	tcre_type_t type;
	char c[UCHAR_MAX];
};
typedef struct tcre tcre_t;

static int amatch(char *subject, tcre_t *tcre);
static int cmatch(tcre_type_t type, char c, tcre_t *tcre);
static int smatch(char *subject, tcre_t *ch, tcre_t *tcre);

static int cmatch(tcre_type_t type, char c, tcre_t *tcre) {
	return (type == tcre[0].type && tcre[0].c[(unsigned char)c]);
}

static int smatch(char *subject, tcre_t *ch, tcre_t *tcre) {
	size_t i;
	for (i = 0; i < strlen(subject); i++) {
		int rc;
		rc = amatch(subject, &tcre[i]);
		if (rc == 1) {
			return 1;
		}

		if (cmatch(ORD, subject[i], ch) && cmatch(DOT, '.', ch)) {
			break;
		}
	}
	return 0;
}

static int amatch(char *subject, tcre_t *tcre) {

	if (cmatch(ORD, '\0', &tcre[0])) {
		return 1; /* end of regex without bailing? success! */
	} else if (cmatch(QUEST, '?', &tcre[1])) {
		if (cmatch(ORD, subject[0], &tcre[0])) {
			return amatch(&subject[1], &tcre[2]);
		} else {
			return amatch(&subject[0], &tcre[2]);
		}
	} else if (cmatch(PLUS, '+', &tcre[1])) {
		if (!cmatch(ORD, subject[0], &tcre[0])) {
			return 0;
		}
		return smatch(subject, &tcre[0],  &tcre[2]);
	} else if (cmatch(STAR, '*', &tcre[1])) {
		return smatch(subject, &tcre[0],  &tcre[2]);
	} else if (cmatch(ORD, '\0', &tcre[1]) && cmatch(RANCHOR, '$', &tcre[0])) {
		return strlen(subject) == 0;
	} else if (strlen(subject) != 0 && (cmatch(DOT, '.', &tcre[0]) || cmatch(ORD, subject[0], &tcre[0]))) {
		return amatch(&subject[1], &tcre[1]);
	} else {
		return 0;
	}
}

static tcre_t *compile(char *pattern) {
	tcre_t *tcre;
	size_t len;
	size_t i, j, k;
	int invert;

	if (pattern == NULL) {
		return NULL;
	}

	len = strlen(pattern) + 1;
#ifdef HAVE_REALLOCARRAY
	tcre = (tcre_t *) reallocarray(NULL, sizeof(tcre_t), len);
#else
	tcre = (tcre_t *) malloc(sizeof(tcre_t) * len);
#endif
	if (tcre == NULL) {
		return NULL;
	}
	memset(tcre, '\0', sizeof(tcre_t) * len);

	for (invert = j = i = 0; i < len; i++, j++) {
		switch (pattern[i]) {
			case '[':
				tcre[j].type = ORD;
				if (pattern[++i] == '^') {
					invert = 1;
				}
				while (pattern[i] != ']') {
					if (pattern[i+1] == '-' && pattern[i+2] != ']') {
						size_t x, y;
						x = pattern[i];
						y = pattern[i+2];

						for (x = pattern[i], y = pattern[i+2]; x <= y; x++) {
							tcre[j].c[(unsigned char)x] = 1;
						}

						i+=2;
					} else {
						tcre[j].c[(unsigned char)pattern[i]] = 1;
						i++;
					}
				}
				if (invert) {
					for (k = 0; k < UCHAR_MAX; k++) {
						tcre[j].c[k] = !tcre[j].c[k];
					}
				}
				break;
			case '?':
				tcre[j].type = QUEST;
				tcre[j].c[(unsigned char)pattern[i]] = 1;
				break;
			case '+':
				tcre[j].type = PLUS;
				tcre[j].c[(unsigned char)pattern[i]] = 1;
				break;
			case '*':
				tcre[j].type = STAR;
				tcre[j].c[(unsigned char)pattern[i]] = 1;
				break;
			case '.':
				tcre[j].type = DOT;
				tcre[j].c[(unsigned char)pattern[i]] = 1;
				break;
			case '^':
				tcre[j].type = LANCHOR;
				tcre[j].c[(unsigned char)pattern[i]] = 1;
				break;
			case '$':
				tcre[j].type = RANCHOR;
				tcre[j].c[(unsigned char)pattern[i]] = 1;
				break;
			case '\\':
				tcre[j].type = ORD;
				i++;
				tcre[j].c[(unsigned char)pattern[i]] = 1;
				break;
			default:
				tcre[j].type = ORD;
				tcre[j].c[(unsigned char)pattern[i]] = 1;
				break;
		}
	}

	return tcre;
}

int match(char *subject, char *pattern) {
	int rc, result;
	size_t i;
	tcre_t *tcre;

	tcre = compile(pattern);

	if (subject == NULL || tcre == NULL) {
		result = 0;
	} else if (strlen(subject) == 0 && cmatch(ORD, '\0', &tcre[0])) {
		result = 1;
	} else if (cmatch(ORD, '\0', &tcre[1]) && (cmatch(LANCHOR, '^', &tcre[0]) || cmatch(RANCHOR, '$', &tcre[0]))) {
		result = 1;
	} else if (cmatch(LANCHOR, '^', &tcre[0])) {
		rc = amatch(subject, &tcre[1]);
		if (rc == 1) {
			result = 1;
		} else {
			result = 0;
		}
	} else {
		result = 0;
		for (i = 0; i < strlen(subject); i++) {
			rc = amatch(&subject[i], tcre);
			if (rc == 1) {
				result = 1;
				break;
			}
		}
	}

	free(tcre);

	return result;
}
