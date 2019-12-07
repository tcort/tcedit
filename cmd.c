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

#include "cmd.h"
#include "error.h"
#include "shell.h"

int tce_dot(struct context *ctx, struct input in) {
	(void) in;
	fprintf(ctx->out, "%lu\n", ctx->dot);
	return 0;
}

int tce_equals(struct context *ctx, struct input in) {
	fprintf(ctx->out, "%lu\n", in.start);
	return 0;
}

int tce_exclaim(struct context *ctx, struct input in) {
	if (in.params == NULL) {
		return -1;
	}
	doshell(in.params);
	fprintf(ctx->out, "!\n");
	return 0;
}

int tce_a(struct context *ctx, struct input in) {
	int rc;
	struct text *t;
	(void) in;

/* TODO append to specific line */

	t = text_read(ctx->in, 1);
	if (t == NULL) {
		return -1;
	}

	rc = text_append(ctx->text, t);
	if (rc == -1) {
		text_free(t);
		return -1;
	}

	ctx->text_dirty = 1;
	ctx->dot = text_count(ctx->text);

	text_free(t);

	return 0;
}

int tce_d(struct context *ctx, struct input in) {
	size_t nlines = text_count(ctx->text);
	if (in.start < 1 || in.start > in.end || in.end > nlines || in.start > nlines) {
		tce_errno = TCE_ERR_BAD_ADDR;
		return -1;
	}

	ctx->dot = text_delete(ctx->text, in.start, in.end);
	ctx->text_dirty = 1;

	return 0;
}

int tce_f(struct context *ctx, struct input in) {
	if (in.params[0] == '\0') {
		if (ctx->filename[0] == '\0') {
			tce_errno = TCE_ERR_FILENAME_NOT_SET;
			return -1;
		}
		fprintf(ctx->out, "%s\n", ctx->filename);
	} else {
		snprintf(ctx->filename, FILENAME_MAX+1, "%s", in.params);
	}

	return 0;
}

int tce_H(struct context *ctx, struct input in) {
	(void) in;
	ctx->help_on = (ctx->help_on == 1) ? 0 : 1;
	return 0;
}

int tce_h(struct context *ctx, struct input in) {
	(void) in;
	fprintf(ctx->out, "%s\n", tce_strerror(tce_errno));
	return 0;
}

int tce_n(struct context *ctx, struct input in) {
	size_t nlines = text_count(ctx->text);
	if (in.start < 1 || in.start > in.end || in.end > nlines || in.start > nlines) {
		tce_errno = TCE_ERR_BAD_ADDR;
		return -1;
	}
	text_write(ctx->text, ctx->out, in.start, in.end, 1);
	ctx->dot = in.end;
	return 0;
}

int tce_P(struct context *ctx, struct input in) {
	(void) in;
	ctx->prompt_on = !ctx->prompt_on;
	return 0;
}

int tce_p(struct context *ctx, struct input in) {
	size_t nlines = text_count(ctx->text);
	if (in.start < 1 || in.start > in.end || in.end > nlines || in.start > nlines) {
		tce_errno = TCE_ERR_BAD_ADDR;
		return -1;
	}
	text_write(ctx->text, ctx->out, in.start, in.end, 0);
	ctx->dot = in.end;
	return 0;
}

int tce_Q(struct context *ctx, struct input in) {
	(void) in;
	ctx->done = 1;
	return 0;
}

int tce_q(struct context *ctx, struct input in) {
	(void) in;
	if (ctx->text_dirty == 1) {
		tce_errno = TCE_ERR_UNSAVED_CHANGES;
		ctx->text_dirty = 0;
		return -1;
	}
	ctx->done = 1;
	return 0;
}

int tce_r(struct context *ctx, struct input in) {

	int rc;
	struct text *t;
	char *filename;
	FILE *input;

	/* use filename parameter or fall back to default filename */
	filename = (in.params[0] == '\0') ? ctx->filename : in.params;
	if (filename[0] == '\0') { /* no parameter and no default */
		tce_errno = TCE_ERR_FILENAME_NOT_SET;
		return -1;
	}

	input = fopen(filename, "r");
	if (input == NULL) {
		tce_errno = TCE_ERR_READ_FAILED;
		return -1;
	}

	t = text_read(input, 0);
	if (t == NULL) {
		return -1;
	}

	rc = text_append(ctx->text, t);
	if (rc == -1) {
		text_free(t);
		fclose(input);
		return -1;
	}

	ctx->text_dirty = 1;
	ctx->dot = text_count(ctx->text);

	text_free(t);
	fclose(input);

	return 0;
}

int tce_w(struct context *ctx, struct input in) {

	char *filename;
	FILE *out;

	/* use filename parameter or fall back to default filename */
	filename = (in.params[0] == '\0') ? ctx->filename : in.params;
	if (filename[0] == '\0') { /* no parameter and no default */
		tce_errno = TCE_ERR_FILENAME_NOT_SET;
		return -1;
	}

	out = fopen(filename, "w");
	if (out == NULL) {
		tce_errno = TCE_ERR_WRITE_FAILED;
		return -1;
	}

	text_write(ctx->text, out, in.start, in.end, 0);
	fclose(out);

	ctx->dot = in.end;
	ctx->text_dirty = 0;

	return 0;
}

struct command commands[NCOMMANDS] = {
	{ '.', tce_dot,		{ ADDR_NONE,		ADDR_NONE } },
	{ '!', tce_exclaim,	{ ADDR_NONE,		ADDR_NONE } },
	{ '=', tce_equals,	{ ADDR_LAST_LINE,	ADDR_NONE } },
	{ 'a', tce_a,		{ ADDR_CURRENT_LINE,	ADDR_NONE } },
	{ 'd', tce_d,		{ ADDR_CURRENT_LINE,	ADDR_CURRENT_LINE } },
        { 'f', tce_f,           { ADDR_NONE,            ADDR_NONE } },
	{ 'H', tce_H,		{ ADDR_NONE,		ADDR_NONE } },
	{ 'h', tce_h,		{ ADDR_NONE,		ADDR_NONE } },
	{ 'n', tce_n,		{ ADDR_CURRENT_LINE,	ADDR_CURRENT_LINE } },
	{ 'P', tce_P,		{ ADDR_NONE,		ADDR_NONE } },
	{ 'p', tce_p,		{ ADDR_CURRENT_LINE,    ADDR_CURRENT_LINE } },
	{ 'Q', tce_Q, 		{ ADDR_NONE,		ADDR_NONE } },
	{ 'q', tce_q, 		{ ADDR_NONE,		ADDR_NONE } },
	{ 'r', tce_r,		{ ADDR_FIRST_LINE,      ADDR_LAST_LINE } },
	{ 'w', tce_w,		{ ADDR_FIRST_LINE,      ADDR_LAST_LINE } }
};
