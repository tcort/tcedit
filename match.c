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
#include <regex.h>
#include <string.h>

#include "match.h"

int match(char *subject, char *pattern, char *result[]) {

	int		i, j;
	int		rc;
	size_t		len;
	regex_t		regex;
	regmatch_t	matches[NMATCHES];

	if (subject == NULL || pattern == NULL) {
		return 0;
	}

	rc = regcomp(&regex, pattern, REG_EXTENDED);
	if (rc != 0) {
		return -1;
	}

	rc = regexec(&regex, subject, NMATCHES, matches, 0);
	if (rc != 0) {
		regfree(&regex);
		return 0;
	}

	for (i = 0; i < NMATCHES; i++) {
		if (matches[i].rm_so == -1) {
			break;
		}
		len = (size_t) matches[i].rm_eo - matches[i].rm_so;
		result[i] = (char *)malloc(len + 1);
		if (result[i] == NULL) {
			for (j = 0; j < i; j++) {
				free(result[j]);
				result[j] = NULL;
			}
			regfree(&regex);
			return -1;
		}
		memset(result[i], '\0', len + 1);
		strncpy(result[i], subject + matches[i].rm_so, len);
	}

	regfree(&regex);
	return i;
}

