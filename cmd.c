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

int tce_equals(struct context *ctx, struct input in) {
	fprintf(ctx->out, "%zu\n", ctx->dot);
	return 0;
}

int tce_H(struct context *ctx, struct input in) {
	ctx->help_on = (ctx->help_on == 1) ? 0 : 1;
	return 0;
}

int tce_h(struct context *ctx, struct input in) {
	fprintf(ctx->out, "%s\n", tce_strerror(tce_errno));
	return 0;
}

int tce_P(struct context *ctx, struct input in) {
	ctx->prompt_on = !ctx->prompt_on;
	return 0;
}

int tce_Q(struct context *ctx, struct input in) {
	ctx->done = 1;
	return 0;
}

int tce_q(struct context *ctx, struct input in) {
	if (ctx->text_dirty == 1) {
		tce_errno = TCE_ERR_UNSAVED_CHANGES;
		ctx->text_dirty = 0;
		return -1;
	}
	ctx->done = 1;
	return 0;
}

struct command commands[NCOMMANDS] = {
	{ .letter = '=', .action = tce_equals },
	{ .letter = 'H', .action = tce_H },
	{ .letter = 'h', .action = tce_h },
	{ .letter = 'P', .action = tce_P },
	{ .letter = 'Q', .action = tce_Q },
	{ .letter = 'q', .action = tce_q }
};
