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
#include <regex.h>
#include <stdlib.h>
#include <string.h>

#include "cmd.h"
#include "error.h"

int tce_equals(struct context *ctx, struct input in);
int tce_exclaim(struct context *ctx, struct input in);
int tce_newline(struct context *ctx, struct input in);
int tce_a(struct context *ctx, struct input in);
int tce_c(struct context *ctx, struct input in);
int tce_d(struct context *ctx, struct input in);
int tce_E(struct context *ctx, struct input in);
int tce_e(struct context *ctx, struct input in);
int tce_f(struct context *ctx, struct input in);
int tce_H(struct context *ctx, struct input in);
int tce_h(struct context *ctx, struct input in);
int tce_i(struct context *ctx, struct input in);
int tce_n(struct context *ctx, struct input in);
int tce_P(struct context *ctx, struct input in);
int tce_p(struct context *ctx, struct input in);
int tce_Q(struct context *ctx, struct input in);
int tce_q(struct context *ctx, struct input in);
int tce_r(struct context *ctx, struct input in);
int tce_s(struct context *ctx, struct input in);
int tce_w(struct context *ctx, struct input in);

/*
 * ($)=
 */
int tce_equals(struct context *ctx, struct input in) {
	if (in.line1 > text_count(ctx->text)) {
		tce_errno = TCE_ERR_BAD_ADDR;
		return -1;
	}
	fprintf(ctx->out, "%lu\n", in.line1);
	/* ctx->dot unchanged */
	return 0;
}

/*
 * !command
 */
int tce_exclaim(struct context *ctx, struct input in) {
	int rc;
	if (in.params == NULL) {
		tce_errno = TCE_ERR_NO_PARAM;
		return -1;
	} else if (ctx->restricted) {
		tce_errno = TCE_ERR_NON_RMODE_CMD;
		return -1;
	}
	rc = system(in.params);
	(void) rc;
	if (ctx->suppress == 0) {
		fprintf(ctx->out, "!\n");
	}
	/* ctx->dot unchanged */
	return 0;
}

/*
 * # commentary
 */
int tce_octothorp(struct context *ctx, struct input in) {
	/* ctx->dot unchanged */
	return 0;
}

/*
 * (.+1)<newline>
 */
int tce_newline(struct context *ctx, struct input in) {
	if (in.line1 > text_count(ctx->text)) {
		tce_errno = TCE_ERR_BAD_ADDR;
		return -1;
	}
	text_write(ctx->text, ctx->out, in.line1, in.line1, 0);
	ctx->dot = in.line1;
	/* ctx->dot set to the addressed line (default is ".+1") */
	return 0;
}

/*
 * (.)a
 */
int tce_a(struct context *ctx, struct input in) {
	int rc;
	struct text *t;

	if (in.line1 < 0 || in.line1 > text_count(ctx->text)) {
		tce_errno = TCE_ERR_BAD_ADDR;
		return -1;
	}

	t = text_read(ctx->in, 1);
	if (t == NULL) {
		tce_errno = TCE_ERR_READ_FAILED;
		return -1;
	}

	rc = text_append(ctx->text, t, in.line1);
	if (rc == -1) {
		tce_errno = TCE_ERR_WRITE_FAILED;
		text_free(t);
		return -1;
	}

	ctx->text_dirty = 1;
	ctx->dot = in.line1 + text_count(t);
	/* ctx->dot = last line appended */

	text_free(t);

	return 0;
}

/*
 * (.,.)c
 */
int tce_c(struct context *ctx, struct input in) {
	int rc;
	struct text *t;

	if (in.line1 < 1 || in.line1 > in.line2 || in.line2 > text_count(ctx->text) || in.line1 > text_count(ctx->text)) {
		tce_errno = TCE_ERR_BAD_ADDR;
		return -1;
	}

	ctx->dot = text_delete(ctx->text, in.line1, in.line2);

	t = text_read(ctx->in, 1);
	if (t == NULL) {
		tce_errno = TCE_ERR_READ_FAILED;
		return -1;
	}

	rc = text_append(ctx->text, t, in.line1 - 1);
	if (rc == -1) {
		tce_errno = TCE_ERR_WRITE_FAILED;
		text_free(t);
		return -1;
	}

	ctx->text_dirty = 1;
	ctx->dot = in.line1 + text_count(t);
	/* ctx->dot = last line appended */

	text_free(t);
	return 0;
}

/*
 * (.,.)d
 */
int tce_d(struct context *ctx, struct input in) {
	if (in.line1 < 1 || in.line1 > in.line2 || in.line2 > text_count(ctx->text) || in.line1 > text_count(ctx->text)) {
		tce_errno = TCE_ERR_BAD_ADDR;
		return -1;
	}

	ctx->dot = text_delete(ctx->text, in.line1, in.line2);
	/* ctx->dot = first line not deleted */
	ctx->text_dirty = 1;

	return 0;
}

/*
 * e file
 */
int tce_E(struct context *ctx, struct input in) {

	FILE *input;
	char *filename;
	struct text *t;
	int rc;

	if (in.params[0] == '\0') {
		tce_errno = TCE_ERR_FILENAME_NOT_SET;
		return -1;
	}

	/* set filename */
	if (in.params[0] == '!') {
		filename = in.params;
	} else {
		snprintf(ctx->filename, FILENAME_MAX+1, "%s", in.params);
		filename = ctx->filename;
	}

	if (ctx->restricted == 1 && (strstr(filename, "..") != NULL ||
					strstr(filename, "/") != NULL)) {
		tce_errno = TCE_ERR_RMODE_FILE;
		return -1;
	}

	/* clear buffer */
	if (text_count(ctx->text) > 0) {
		text_delete(ctx->text, 1, text_count(ctx->text));
		ctx->dot = 0;
	}

	/* read */
	if (filename[0] == '!') {
		if (ctx->restricted == 1) {
			tce_errno = TCE_ERR_RMODE_PROC_IO;
			return -1;
		}
		input = popen(&filename[1], "r");
	} else {
		input = fopen(filename, "r");
	}
	if (input == NULL) {
		tce_errno = TCE_ERR_READ_FAILED;
		return -1;
	}

	t = text_read(input, 0);
	if (t == NULL) {
		tce_errno = TCE_ERR_READ_FAILED;
		return -1;
	}

	if (ctx->suppress == 0) {
		fprintf(ctx->out, "%lu\n", t->bytes);
	}

	rc = text_append(ctx->text, t, in.line1);
	if (rc == -1) {
		tce_errno = TCE_ERR_WRITE_FAILED;
		text_free(t);
		fclose(input);
		return -1;
	}

	ctx->text_dirty = 0;
	ctx->dot = text_count(ctx->text) - text_count(t) + in.line1;
	/* ctx->dot = last line read */

	text_free(t);
	if (filename[0] == '!') {
		pclose(input);
	} else {
		fclose(input);
	}

	return 0;
}

/*
 * e file
 */
int tce_e(struct context *ctx, struct input in) {
	if (ctx->text_dirty == 1) {
		tce_errno = TCE_ERR_UNSAVED_CHANGES;
		return -1;
	}
	return tce_E(ctx, in);
}

/*
 * f [file]
 */
int tce_f(struct context *ctx, struct input in) {
	if (in.params[0] == '\0') {
		if (ctx->filename[0] == '\0') {
			tce_errno = TCE_ERR_FILENAME_NOT_SET;
			return -1;
		}
		fprintf(ctx->out, "%s\n", ctx->filename);
	} else {
		if (ctx->restricted == 1 && (strstr(in.params, "..") != NULL ||
						strstr(in.params, "/") != NULL)) {
			tce_errno = TCE_ERR_RMODE_FILE;
			return -1;
		}
		snprintf(ctx->filename, FILENAME_MAX+1, "%s", in.params);
	}
	/* ctx->dot unchanged */
	return 0;
}

/*
 * H
 */
int tce_H(struct context *ctx, struct input in) {
	(void) in;
	ctx->help_on = (ctx->help_on == 1) ? 0 : 1;
	/* ctx->dot unchanged */
	return 0;
}

/*
 * h
 */
int tce_h(struct context *ctx, struct input in) {
	(void) in;
	fprintf(ctx->out, "%s\n", tce_strerror(tce_errno));
	/* ctx->dot unchanged */
	return 0;
}

/*
 * (.)i
 */
int tce_i(struct context *ctx, struct input in) {
	int rc;
	struct text *t;

	if (in.line1 < 1 || in.line1 > in.line2 || in.line2 > text_count(ctx->text) || in.line1 > text_count(ctx->text)) {
		tce_errno = TCE_ERR_BAD_ADDR;
		return -1;
	}

	t = text_read(ctx->in, 1);
	if (t == NULL) {
		tce_errno = TCE_ERR_READ_FAILED;
		return -1;
	}

	rc = text_append(ctx->text, t, in.line1 - 1);
	if (rc == -1) {
		tce_errno = TCE_ERR_WRITE_FAILED;
		text_free(t);
		return -1;
	}

	ctx->text_dirty = 1;
	ctx->dot = in.line1 + text_count(t);
	/* ctx->dot = last line inserted */

	text_free(t);

	return 0;
}

/*
 * (.,.)n
 */
int tce_n(struct context *ctx, struct input in) {
	if (in.line1 < 1 || in.line1 > in.line2 || in.line2 > text_count(ctx->text) || in.line1 > text_count(ctx->text)) {
		tce_errno = TCE_ERR_BAD_ADDR;
		return -1;
	}

	text_write(ctx->text, ctx->out, in.line1, in.line2, 1);
	ctx->dot = in.line2;
	/* ctx->dot = last line printed */
	return 0;
}

/*
 * P
 */
int tce_P(struct context *ctx, struct input in) {
	(void) in;
	ctx->prompt_on = !ctx->prompt_on;
	/* ctx->dot unchanged */
	return 0;
}

/*
 * (.,.)p
 */
int tce_p(struct context *ctx, struct input in) {
	if (in.line1 < 1 || in.line1 > in.line2 || in.line2 > text_count(ctx->text) || in.line1 > text_count(ctx->text)) {
		tce_errno = TCE_ERR_BAD_ADDR;
		return -1;
	}
	text_write(ctx->text, ctx->out, in.line1, in.line2, 0);
	ctx->dot = in.line2;
	/* ctx->dot = last line printed */
	return 0;
}

/*
 * Q
 */
int tce_Q(struct context *ctx, struct input in) {
	(void) in;
	ctx->done = 1;
	/* ctx->dot unchanged */
	return 0;
}

/*
 * q
 */
int tce_q(struct context *ctx, struct input in) {
	(void) in;
	if (ctx->text_dirty == 1) {
		tce_errno = TCE_ERR_UNSAVED_CHANGES;
		ctx->text_dirty = 0;
		return -1;
	}
	ctx->done = 1;
	/* ctx->dot unchanged */
	return 0;
}

/*
 * ($)r file
 * ($)r !command
 */
int tce_r(struct context *ctx, struct input in) {

	int rc;
	struct text *t;
	char *filename;
	FILE *input;

	if (in.line1 < 0 || in.line1 > text_count(ctx->text)) {
		tce_errno = TCE_ERR_BAD_ADDR;
		return -1;
	}

	/* use filename parameter or fall back to default filename */
	filename = (in.params[0] == '\0') ? ctx->filename : in.params;
	if (filename[0] == '\0') { /* no parameter and no default */
		tce_errno = TCE_ERR_FILENAME_NOT_SET;
		return -1;
	}

	if (ctx->restricted == 1 && (strstr(filename, "..") != NULL ||
					strstr(filename, "/") != NULL)) {
		tce_errno = TCE_ERR_RMODE_FILE;
		return -1;
	}

	if (filename[0] == '!') {
		if (ctx->restricted == 1) {
			tce_errno = TCE_ERR_RMODE_PROC_IO;
			return -1;
		}
		input = popen(&filename[1], "r");
	} else {
		input = fopen(filename, "r");
	}
	if (input == NULL) {
		tce_errno = TCE_ERR_READ_FAILED;
		return -1;
	}

	t = text_read(input, 0);
	if (t == NULL) {
		tce_errno = TCE_ERR_READ_FAILED;
		return -1;
	}

	if (ctx->suppress == 0) {
		fprintf(ctx->out, "%lu\n", t->bytes);
	}

	rc = text_append(ctx->text, t, in.line1);
	if (rc == -1) {
		tce_errno = TCE_ERR_WRITE_FAILED;
		text_free(t);
		fclose(input);
		return -1;
	}

	ctx->text_dirty = 1;
	ctx->dot = text_count(ctx->text) - text_count(t) + in.line1;
	/* ctx->dot = last line read */

	text_free(t);
	if (filename[0] == '!') {
		pclose(input);
	} else {
		fclose(input);
	}

	return 0;
}

/*
 * (.,.)s/pattern/replacement/
 */
int tce_s(struct context *ctx, struct input in) {

	int rc;
	size_t i;
	char pattern[LINE_MAX];
	char replacement[LINE_MAX];
	regex_t preg;
	regmatch_t matches[8];

	if (in.line1 < 1 || in.line1 > in.line2 || in.line2 > text_count(ctx->text) || in.line1 > text_count(ctx->text)) {
		tce_errno = TCE_ERR_BAD_ADDR;
		return -1;
	}

	rc = regcomp(&preg, "/([^\\/]*)/([^\\/]*)/", REG_EXTENDED);
	if (rc != 0) {
		regfree(&preg);
		tce_errno = TCE_ERR_BAD_PARAM;
		return -1;
	}

	rc = regexec(&preg, in.params, 8, matches, 0);
	if (rc != 0) {
		regfree(&preg);
		tce_errno = TCE_ERR_BAD_PARAM;
		return -1;
	}

	regfree(&preg);

	memset(pattern, '\0', LINE_MAX);
	if (matches[1].rm_eo - matches[1].rm_so > 0 && matches[1].rm_eo - matches[1].rm_so < LINE_MAX) {
		snprintf(pattern, matches[1].rm_eo - matches[1].rm_so + 1, "%s", &in.params[matches[1].rm_so]);
	}

	memset(replacement, '\0', LINE_MAX);
	if (matches[2].rm_eo - matches[2].rm_so > 0 && matches[2].rm_eo - matches[2].rm_so < LINE_MAX) {
		snprintf(replacement, matches[2].rm_eo - matches[2].rm_so + 1, "%s", &in.params[matches[2].rm_so]);
	}

	for (i = in.line1; i <= in.line2; i++) {
		int r;
		r = text_substitute(ctx->text, pattern, replacement, i);
		if (r == -1) {
			tce_errno = TCE_ERR_BAD_SUB;
			return -1;
		}
		rc+=r;
	}

	if (rc > 0) {
		ctx->text_dirty = 1;
		ctx->dot = in.line1;
	}

	return 0;
}

/*
 * (1,$)w file
 * (1,$)w !command
 */
int tce_w(struct context *ctx, struct input in) {

	char *filename;
	FILE *out;

	if (in.line1 < 1 || in.line1 > in.line2 || in.line2 > text_count(ctx->text) || in.line1 > text_count(ctx->text)) {
		tce_errno = TCE_ERR_BAD_ADDR;
		return -1;
	}

	/* use filename parameter or fall back to default filename */
	filename = (in.params[0] == '\0') ? ctx->filename : in.params;
	if (filename[0] == '\0') { /* no parameter and no default */
		tce_errno = TCE_ERR_FILENAME_NOT_SET;
		return -1;
	}

	if (ctx->restricted == 1 && (strstr(filename, "..") != NULL ||
					strstr(filename, "/") != NULL)) {
		tce_errno = TCE_ERR_RMODE_FILE;
		return -1;
	}

	if (filename[0] == '!') {
		if (ctx->restricted == 1) {
			tce_errno = TCE_ERR_RMODE_PROC_IO;
			return -1;
		}
		out = popen(&filename[1], "w");
	} else {
		out = fopen(filename, "w");
	}
	if (out == NULL) {
		tce_errno = TCE_ERR_WRITE_FAILED;
		return -1;
	}

	text_write(ctx->text, out, in.line1, in.line2, 0);
	if (filename[0] == '!') {
		pclose(out);
	} else {
		fclose(out);
	}

	if (ctx->suppress == 0) {
		fprintf(ctx->out, "%lu\n", ctx->text->bytes);
	}

	ctx->text_dirty = 0;
	/* ctx->dot unchanged */

	return 0;
}

int exec_cmd(struct context *ctx, struct input in) {
	int rc, i;
	rc = -1;
	for (i = 0; i < NCOMMANDS; i++) {
		if (commands[i].letter == in.letter) {
			if (in.line1 == 0) {
				switch (commands[i].default_addrs[0]) {
					case ADDR_FIRST_LINE: 	in.line1 = 1; break;
					case ADDR_CURRENT_LINE:	in.line1 = ctx->dot; break;
					case ADDR_LAST_LINE:	in.line1 = text_count(ctx->text); break;
					case ADDR_NEXT_LINE:	in.line1 = ctx->dot + 1; break;
					case ADDR_NONE:		in.line1 = 0; break;
					default:		in.line1 = 0; break;
				}
			}

			if (in.line2 == 0) {
				switch (commands[i].default_addrs[1]) {
					case ADDR_FIRST_LINE: 	in.line2 = 1; break;
					case ADDR_CURRENT_LINE:	in.line2 = ctx->dot; break;
					case ADDR_LAST_LINE:	in.line2 = text_count(ctx->text); break;
					case ADDR_NEXT_LINE:	in.line2 = ctx->dot + 1; break;
					case ADDR_NONE:		in.line2 = 0; break;
					default:		in.line2 = 0; break;
				}

			}

			rc = commands[i].action(ctx, in);
			break;
		}
	}
	return rc;
}

struct command commands[NCOMMANDS] = {
	{ '!', tce_exclaim,	{ ADDR_NONE,		ADDR_NONE } },
	{ '=', tce_equals,	{ ADDR_LAST_LINE,	ADDR_NONE } },
	{ '#', tce_octothorp,	{ ADDR_NONE,		ADDR_NONE } },
	{ '\n', tce_newline,	{ ADDR_NEXT_LINE,	ADDR_NONE } },
	{ 'a', tce_a,		{ ADDR_CURRENT_LINE,	ADDR_NONE } },
	{ 'c', tce_c,		{ ADDR_CURRENT_LINE,	ADDR_CURRENT_LINE } },
	{ 'd', tce_d,		{ ADDR_CURRENT_LINE,	ADDR_CURRENT_LINE } },
	{ 'E', tce_E,           { ADDR_NONE,            ADDR_NONE } },
	{ 'e', tce_e,           { ADDR_NONE,            ADDR_NONE } },
	{ 'f', tce_f,           { ADDR_NONE,            ADDR_NONE } },
	{ 'H', tce_H,		{ ADDR_NONE,		ADDR_NONE } },
	{ 'h', tce_h,		{ ADDR_NONE,		ADDR_NONE } },
	{ 'i', tce_i,		{ ADDR_CURRENT_LINE,	ADDR_NONE } },
	{ 'n', tce_n,		{ ADDR_CURRENT_LINE,	ADDR_CURRENT_LINE } },
	{ 'P', tce_P,		{ ADDR_NONE,		ADDR_NONE } },
	{ 'p', tce_p,		{ ADDR_CURRENT_LINE,    ADDR_CURRENT_LINE } },
	{ 'Q', tce_Q, 		{ ADDR_NONE,		ADDR_NONE } },
	{ 'q', tce_q, 		{ ADDR_NONE,		ADDR_NONE } },
	{ 'r', tce_r,		{ ADDR_LAST_LINE,       ADDR_NONE } },
	{ 's', tce_s,		{ ADDR_CURRENT_LINE,	ADDR_CURRENT_LINE } },
	{ 'w', tce_w,		{ ADDR_FIRST_LINE,      ADDR_LAST_LINE } }
};


