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

#include <ctype.h>
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ctx.h"
#include "parse.h"
#include "text.h"

static size_t resolve_regex(struct context *ctx, char *pattern, size_t n) {
	size_t i = ctx->dot;
	char *subject;

	char *regex;
#ifdef HAVE_REALLOCARRAY
	regex = (char *) reallocarray(NULL, n+1, sizeof(char));
#else
	regex = (char *) malloc((n+1) * sizeof(char));
#endif
	if (regex == NULL) {
		return ctx->dot;
	}
	memset(regex, '\0', n+1);
	snprintf(regex, n, "%s", pattern+1);

	if (pattern[0] == '/') { /* forward search */
		i = text_search_fwd(ctx->text, regex, ctx->dot, text_count(ctx->text));
	} else { /* reverse search */
		i = text_search_rev(ctx->text, regex, ctx->dot, 1);
	}

	free(regex);

	return i;
}

struct input parse(struct context *ctx, char *line) {
	struct input in;
	char *p;

	memset(&in, '\0', sizeof(struct input));

	p = line;

	in.line1 = (unsigned long) atoi(p);
	while (p != NULL && isdigit(*p)) {
		++p;
	}

	if (*p == '/' || *p == '?') {
		size_t n;
		for (n = 1; n < strlen(p); n++) {
			if ((p[n] == '/' || p[n] == '?') &&
				(n == 0 || p[n-1] != '\\')) {
				break;
			}
		}
		in.line1 = resolve_regex(ctx, p, n);
		p+=n+1;
	} else if (*p == '$') {
		in.line1 = text_count(ctx->text);
		p++;
	} else if (*p == '.') {
		in.line1 = ctx->dot;
		p++;
	}

	if (*p == ',' || *p == '%') {
		in.comma = 1;
		++p;
	}

	in.line2 = (unsigned long) atoi(p);
	while (p != NULL && isdigit(*p)) {
		++p;
	}

	if (*p == '/' || *p == '?') {
		size_t n;
		for (n = 1; n < strlen(p); n++) {
			if ((p[n] == '/' || p[n] == '?') &&
				(n == 0 || p[n-1] != '\\')) {
				break;
			}
		}
		in.line2 = resolve_regex(ctx, p, n);
		p+=n+1;
	} else if (*p == '$') {
		in.line2 = text_count(ctx->text);
		p++;
	} else if (*p == '.') {
		in.line2 = ctx->dot;
		p++;
	}

	in.letter = *p;
	++p;
	while (p != NULL && *p == ' ') {
		++p;
	}
	in.params = strdup(p);
	for (p = in.params; *p != '\0'; p++) {
		if (*p == '\n') {
			*p = '\0';
		}
	}

	/* , -> 1,$ */
	if (in.line2 == 0 && in.line1 == 0 && in.comma == 1) {
		in.line1 = 1;
		in.line2 = text_count(ctx->text);
	/* ,N -> 1,N */
	} else if (in.line1 == 0 && in.comma && in.line2 != 0) {
		in.line1 = 1;
	/* N, -> N,N */
	} else if (in.line1 != 0 && in.comma && in.line2 == 0) {
		in.line2 = in.line1;
	}

	return in;
}
