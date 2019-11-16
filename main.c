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

#include "buffer.h"
#include "ctx.h"
#include "cmd.h"
#include "error.h"
#include "io.h"
#include "parse.h"

int main(int argc, char *argv[]) {

	int		i;
	int		rc;
	int		optc;
	const char     *const short_options = "p:v";
	struct input	in;
	struct context	ctx = {
		.in = stdin,
		.out = stdout,
		.dot = 0,
		.help_on = 0,
		.prompt_on = 0,
		.prompt = "*",
		.text_dirty = 0,
		.done = 0,
		.text = {
			.buf = bf_alloc(8192, 4096),
			.head = NULL,
			.tail = NULL
		}
	};

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

	/* playpen for debugging/testing */
	printf("%zu\n", ln_count(&ctx.text));
	ln_append(&ctx.text, "Hello");
	printf("%zu\n", ln_count(&ctx.text));
	ln_append(&ctx.text, "Bonjour");
	printf("%zu\n", ln_count(&ctx.text));
	ln_append(&ctx.text, "Hola");
printf("--\n");
	ln_print(ctx.out, &ctx.text, 2, 3, 1);
printf("--\n");
	printf("%zu\n", ln_count(&ctx.text));
	printf("%s\n", ln_getline(&ctx.text, 2)->pos);
	printf("%d\n", tce_nerror);
	/* end of playpen */

	do {
		char *cmd = readaline(ctx.in, ctx.out, ctx.prompt_on ? ctx.prompt : "");
		if (cmd == NULL) {
			break;
		}
		in = parse(cmd);
		rc = -1;
		for (i = 0; i < NCOMMANDS; i++) {
			if (commands[i].letter == in.letter) {
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
	} while (!ctx.done);


	ctx_free(&ctx);

	return 0;
}
