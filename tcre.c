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

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tcre.h"

struct tcre {
	char c[256];
};
typedef struct tcre tcre_t;

static int amatch(char *subject, char *pattern);
static int smatch(char *subject, char ch, char *pattern);

static int smatch(char *subject, char ch, char *pattern) {
	size_t i;
	for (i = 0; i < strlen(subject); i++) {
		int rc;
		rc = amatch(subject, &pattern[i]);
		if (rc == 1) {
			return 1;
		}

		if (subject[i] != ch && '.' != ch) {
			break;
		}
	}
	return 0;
}

static int amatch(char *subject, char *pattern) {

	if (strlen(pattern) == 0) {
		return 1; /* end of regex without bailing? success! */
	} else if (pattern[0] == '\\') {
		if (subject[0] == pattern[1]) {
			return amatch(&subject[1], &pattern[2]);
		} else {
			return 0;
		}
	} else if (pattern[1] == '?') {
		if (subject[0] == pattern[0]) {
			return amatch(&subject[1], &pattern[2]);
		} else {
			return amatch(&subject[0], &pattern[2]);
		}
	} else if (pattern[1] == '+') {
		if (subject[0] != pattern[0]) {
			return 0;
		}
		return smatch(subject, pattern[0],  &pattern[2]);
	} else if (pattern[1] == '*') {
		return smatch(subject, pattern[0],  &pattern[2]);
	} else if (strlen(pattern) == 1 && pattern[0] == '$') {
		return strlen(subject) == 0;
	} else if (strlen(subject) != 0 && (pattern[0] == '.' || subject[0] == pattern[0])) {
		return amatch(&subject[1], &pattern[1]);
	} else {
		return 0;
	}
}

static tcre_t *compile(char *pattern) {
	tcre_t *tcre;
	size_t len;
	size_t i;

	if (pattern == NULL) {
		return NULL;
	}

	len = strlen(pattern) + 1;
	tcre = (tcre_t *) reallocarray(NULL, sizeof(tcre_t), len);
	if (tcre == NULL) {
		return NULL;
	}
	memset(tcre, '\0', sizeof(tcre_t) * len);

	for (i = 0; i < len; i++) {
		tcre[i].c[0] = pattern[i];
	}

	return tcre;
}

int match(char *subject, char *pattern) {
	int rc, result;
	size_t i;
	tcre_t *tcre;

	tcre = compile(pattern);
	if (tcre == NULL) {
		return 0;
	}


	if (subject == NULL || pattern == NULL) {
		result = 0;
	} else if (strlen(subject) == 0 && strlen(pattern) == 0) {
		result = 1;
	} else if (strlen(pattern) == 1 && (pattern[0] == '^' || pattern[0] == '$')) {
		result = 1;
	} else if (pattern[0] == '^') {
		rc = amatch(subject, &pattern[1]);
		if (rc == 1) {
			result = 1;
		} else {
			result = 0;
		}
	} else {
		result = 0;
		for (i = 0; i < strlen(subject); i++) {
			rc = amatch(&subject[i], pattern);
			if (rc == 1) {
				result = 1;
				break;
			}
		}
	}

	free(tcre);

	return result;
}