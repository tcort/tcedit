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

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ctx.h"
#include "cmd.h"
#include "error.h"
#include "io.h"
#include "parse.h"
#include "text.h"

int main(int argc, char *argv[]) {

	int i;
	int rc;
	int optc;
	char *cmd;
	const char *const short_options = "p:rsv";
	struct input in;
	struct context ctx;

	ctx.in = stdin,
	ctx.out = stdout,
	ctx.dot = 0,
	ctx.help_on = 0,
	ctx.prompt_on = 0,
	ctx.prompt = "*",
	ctx.text_dirty = 0,
	ctx.done = 0,
	ctx.text = text_new();
	ctx.filename[0] = '\0'; /* default to not set */
	ctx.restricted = 0;
	ctx.suppress = 0;

	if (argc > 0 && argv[0] != NULL) {
		char *bname = basename(argv[0]);
		ctx.restricted = bname[0] == 'r';
	}

	while ((optc = getopt(argc, argv, short_options)) != -1) {
		switch (optc) {
			case 'p':
				ctx.prompt = optarg;
				ctx.prompt_on = 1;
				break;
			case 'r':
				ctx.restricted = 1;
				break;
			case 's':
				ctx.suppress = 1;
				break;
			case 'v':
				fprintf(ctx.out, "%s\n", PACKAGE_STRING);
				text_free(ctx.text);
				exit(EXIT_SUCCESS);
				break;
			default:
				text_free(ctx.text);
				exit(EXIT_FAILURE);
		}
	}

	argc -= optind;
	argv += optind;

	if (argc > 0) {
		cmd = NULL;
		in.letter = 'e';
		in.params = strdup(argv[0]);
		in.line1 = 0;
		in.line2 = 0;
		goto exec_cmd;
	}

	do {
		cmd = readaline(ctx.in, ctx.out, ctx.prompt_on ? ctx.prompt : "");
		if (cmd == NULL) {
			break;
		}
		in = parse(&ctx, cmd);
exec_cmd:
		rc = -1;
		for (i = 0; i < NCOMMANDS; i++) {
			if (commands[i].letter == in.letter) {
				if (in.line1 == 0) {
					switch (commands[i].default_addrs[0]) {
						case ADDR_FIRST_LINE: 	in.line1 = 1; break;
						case ADDR_CURRENT_LINE:	in.line1 = ctx.dot; break;
						case ADDR_LAST_LINE:	in.line1 = text_count(ctx.text); break;
						case ADDR_NEXT_LINE:	in.line1 = ctx.dot + 1; break;
						case ADDR_NONE:		in.line1 = 0; break;
						default:		in.line1 = 0; break;
					}
				}

				if (in.line2 == 0) {
					switch (commands[i].default_addrs[1]) {
						case ADDR_FIRST_LINE: 	in.line2 = 1; break;
						case ADDR_CURRENT_LINE:	in.line2 = ctx.dot; break;
						case ADDR_LAST_LINE:	in.line2 = text_count(ctx.text); break;
						case ADDR_NEXT_LINE:	in.line2 = ctx.dot + 1; break;
						case ADDR_NONE:		in.line2 = 0; break;
						default:		in.line2 = 0; break;
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
		if (cmd != NULL) {
			free(cmd);
		}

		if (feof(ctx.in) || ferror(ctx.in)) {
			ctx.done = 1;
		}
	} while (!ctx.done);

	text_free(ctx.text);

	return 0;
}
