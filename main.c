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

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"

#include "ctx.h"
#include "cmd.h"
#include "error.h"
#include "io.h"
#include "parse.h"
#include "sig.h"
#include "text.h"

int main(int argc, char *argv[]) {

	int		i;
	int		rc;
	int		optc;
	const char     *const short_options = "p:v";
	struct input	in;
	struct context	ctx;

	ctx.in = stdin,
	ctx.out = stdout,
	ctx.dot = 0,
	ctx.help_on = 0,
	ctx.prompt_on = 0,
	ctx.prompt = "*",
	ctx.text_dirty = 0,
	ctx.done = 0,
	ctx.text = text_new();

	siginit();

	while ((optc = getopt(argc, argv, short_options)) != -1) {
		switch (optc) {
			case 'p':
				ctx.prompt = optarg;
				ctx.prompt_on = 1;
				break;
			case 'v':
				fprintf(ctx.out, "%s\n", PACKAGE_STRING);
				ctx_free(&ctx);
				exit(EXIT_SUCCESS);
				break;
			default:
				ctx_free(&ctx);
				exit(EXIT_FAILURE);
		}
	}

	argc -= optind;
	argv += optind;

	do {
		char *cmd = readaline(ctx.in, ctx.out, ctx.prompt_on ? ctx.prompt : "");
		if (cmd == NULL) {
			break;
		}
		in = parse(&ctx, cmd);
		rc = -1;
		for (i = 0; i < NCOMMANDS; i++) {
			if (commands[i].letter == in.letter) {
				if (in.start == 0) {
					switch (commands[i].default_addrs[0]) {
						case ADDR_FIRST_LINE: 	in.start = 1; break;
						case ADDR_CURRENT_LINE:	in.start = ctx.dot; break;
						case ADDR_LAST_LINE:	in.start = text_count(ctx.text); break;
						case ADDR_NEXT_LINE:	in.start = ctx.dot + 1; break;
						case ADDR_NONE:		in.start = 0; break;
						default:		in.start = 0; break;
					}
				}

				if (in.end == 0) {
					switch (commands[i].default_addrs[1]) {
						case ADDR_FIRST_LINE: 	in.end = 1; break;
						case ADDR_CURRENT_LINE:	in.end = ctx.dot; break;
						case ADDR_LAST_LINE:	in.end = text_count(ctx.text); break;
						case ADDR_NEXT_LINE:	in.end = ctx.dot + 1; break;
						case ADDR_NONE:		in.end = 0; break;
						default:		in.end = 0; break;
					}

				}

				rc = commands[i].action(&ctx, in);
				break;
			}
		}
		if (rc != 0) {
			if (ctx.help_on == 1) {
				fprintf(ctx.out, "%s\n", tce_strerror(tce_errno));
			}
			fprintf(ctx.out, "?\n");
		}
		if (in.params != NULL) {
			free(in.params);
			in.params = NULL;
		}
		free(cmd);

		if (feof(ctx.in) || ferror(ctx.in)) {
			ctx.done = 1;
		}
	} while (!ctx.done);


	ctx_free(&ctx);

	return 0;
}
