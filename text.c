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

#include <errno.h>
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif
#include <regex.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "dstring.h"
#include "error.h"
#include "text.h"

struct text *text_new(void) {
	struct text *t;

#ifdef HAVE_REALLOCARRAY
	t = reallocarray(NULL, 1, sizeof(struct text));
#else
	t = malloc(sizeof(struct text));
#endif
	if (t == NULL) {
		return NULL;
	}

	strcpy(t->tmpfile, "/tmp/tcedit.XXXXXX");
	t->scratchfd = mkstemp(t->tmpfile);
	if (t->scratchfd == -1) {
		free(t);
		return NULL;
	}

	t->scratch = fdopen(t->scratchfd, "w+");
	if (t->scratch == NULL) {
		close(t->scratchfd);
		unlink(t->tmpfile);
		return NULL;
	}

	t->length = 0; /* assume 4K pages */
	t->incr = 4096/sizeof(long);
	t->capacity = 4096/sizeof(long);

#ifdef HAVE_REALLOCARRAY
	t->lines = (long *) reallocarray(NULL, t->capacity, sizeof(long));
#else
	t->lines = (long *) malloc(sizeof(long) * t->capacity);

#endif
	if (t->lines == NULL) {
		fclose(t->scratch);
		close(t->scratchfd);
		unlink(t->tmpfile);
		return NULL;
	}

	t->line = NULL;
	t->cap = 0;
	t->len = 0;

	return t;
}

size_t text_count(struct text *t) {
	return t->length;
}

size_t text_delete(struct text *t, size_t begin, size_t end) {
	size_t i;
	size_t diff = end - begin + 1;
	for (i = begin; i + diff <= text_count(t); i++) {
		t->lines[i] = t->lines[i+diff];
	}
	t->length = t->length - diff;
	return begin;
}

void text_extend(struct text *t) {
	t->capacity += t->incr;
#ifdef HAVE_REALLOCARRAY
	t->lines = (long *) reallocarray(t->lines, t->capacity, sizeof(long));
#else
	t->lines = (long *) realloc(t->lines, t->capacity * sizeof(long));
#endif
}

int text_appendln(struct text *t, char *s, size_t where) {

	size_t i;
	size_t nitems;

	where++; /* insert *after* where */

	if (t->length + 1 >= t->capacity) {
		text_extend(t);
	}

	t->length++;
	for (i = t->length; i > where; i--) {
		t->lines[i] = t->lines[i-1];
	}

	fseek(t->scratch, 0, SEEK_END);
	t->lines[where] = ftell(t->scratch);
	nitems = fwrite(s, sizeof(char), strlen(s), t->scratch);
	return nitems == strlen(s) ? 0 : -1;
}

int text_append(struct text *t, struct text *tin, size_t where) {

	size_t i;

	for (i = 0; i < tin->length; i++) {
		int rc;
		rc = text_appendln(t, text_getln(tin, i+1), where+i);
		if (rc == -1) {
			return -1;
		}
	}

	return 0;
}

int text_match(struct text *t, char *pattern, size_t where) {

	int rc;
	char *subject;
	regex_t preg;

	subject = text_getln(t, where);
	if (subject == NULL) {
		return -1;
	}

	rc = regcomp(&preg, pattern, REG_EXTENDED | REG_NOSUB);
	if (rc != 0) {
		return -1;
	}

	rc = regexec(&preg, subject, 0, NULL, 0);
	regfree(&preg);

	return (rc == 0);
}

size_t text_search_fwd(struct text *t, char *regex, size_t begin, size_t end) {
	size_t i;
	for (i = begin; i <= end && i <= text_count(t); i++) {
		if (text_match(t, regex, i) == 1) {
			return i;
		}
	}
	return 0;
}

size_t text_search_rev(struct text *t, char *regex, size_t begin, size_t end) {
	size_t i;
	for (i = begin; i >= end && i >= 1; i--) {
		if (text_match(t, regex, i) == 1) {
			return i;
		}
	}
	return 0;
}

int text_substitute(struct text *t, char *pattern, char *replacement,
						size_t where) {

	int rc;
	char *subject;
	regex_t preg;
	regmatch_t matches[1];
	struct dstring ds;

	subject = text_getln(t, where);
	if (subject == NULL) {
		return -1;
	}

	rc = regcomp(&preg, pattern, REG_EXTENDED);
	if (rc != 0) {
		return -1;
	}

	ds = dstring_new();

	while (regexec(&preg, subject, 1, matches, 0) == 0) {
		dstring_append(&ds, subject, matches[0].rm_so);
		dstring_append(&ds, replacement, strlen(replacement));

		subject = &subject[matches[0].rm_eo];
	}

	dstring_append(&ds, subject, strlen(subject));
	dstring_append(&ds, "\0", 1);

	text_putln(t, where, ds.s);

	dstring_free(&ds);
	regfree(&preg);

	return 1;
}

struct text *text_read(FILE *in, int period_ends_input) {

	struct text *t;
	size_t i;

	t = text_new();
	if (t == NULL) {
		return NULL;
	}

	t->bytes = 0;
	for (i = 0; (t->len = getline(&t->line, &t->cap, in)) > 0; i++) {
		int rc;

		if (t->line[0] == '.' && t->line[1] == '\n' && period_ends_input == 1) {
			break;
		} 

		rc = text_appendln(t, t->line, i);
		if (rc == -1) {
			text_free(t);
			return NULL;
		}

		t->bytes += strlen(t->line);
	}

	return t;
}

void text_write(struct text *t, FILE *out, size_t begin, size_t end, int show_lineno) {

	size_t i;
	t->bytes = 0;
	for (i = begin; i <= t->length && i <= end; i++) {
		char *s;
		s = text_getln(t, i);
		if (show_lineno) {
			fprintf(out, "%ld\t", i);
		}
		fwrite(s, strlen(s), 1, out);
		t->bytes += strlen(s);
	}

	fflush(out);

}

char *text_getln(struct text *t, size_t lineno) {

	fseek(t->scratch, t->lines[lineno], SEEK_SET);
	t->len = getline(&t->line, &t->cap, t->scratch);
	fseek(t->scratch, 0, SEEK_END);
	if (t->len < 0) {
		return NULL;
	}

	return t->line;
}

int text_putln(struct text *t, size_t lineno, char *s) {
	size_t nitems;
	fseek(t->scratch, 0, SEEK_END);
	t->lines[lineno] = ftell(t->scratch);
	nitems = fwrite(s, sizeof(char), strlen(s), t->scratch);
	return nitems == strlen(s) ? 0 : -1;
}

void text_free(struct text *t) {

	if (t->line != NULL) {
		free(t->line);
	}

	if (t->lines != NULL) {
		free(t->lines);
	}

	if (t->scratch != NULL) {
		fclose(t->scratch);
	}

	if (t->scratchfd >= 0) {
		close(t->scratchfd);
	}

	if (t->tmpfile[0] != '\0') {
		unlink(t->tmpfile);
	}

	memset(t, '\0', sizeof(struct text));

	if (t != NULL) {
		free(t);
	}
}
