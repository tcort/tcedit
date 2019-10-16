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

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

static int is_interactive(FILE *f) {
	int fd = fileno(f);
	return (fd == -1) ? 0 : isatty(fd);
}

char *readaline(FILE *input, FILE *output, char *prompt) {

	int i;
	int len = LINE_MAX + 1;
	char *line;

	line = (char *) malloc(len);
	bzero(line, len);

	if (prompt != NULL && is_interactive(output)) {
		fprintf(output, "%s", prompt);
		fflush(output);
	}

	for (i = 0; !feof(input) && !ferror(input) && i < (len - 1); i++) {
		line[i] = fgetc(input);
		if (line[i] == '\n') {
			line[i] = '\0';
			break;
		}
	}

	return line;
}

