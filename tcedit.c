/*
 * tcedit - a line oriented text editor.
 * Copyright (C) 2018  Thomas Cort <linuxgeek@gmail.com>
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


static void print_help(char *progname) {

	fprintf(stdout, "%s - a line oriented text editor\n", PACKAGE_NAME);
	fprintf(stdout, "\n");
	fprintf(stdout, "Usage: %s\n", progname);
	fprintf(stdout, "\n");
	fprintf(stdout, "Options:\n");
	fprintf(stdout, " -h, -?            --help                  Print a helpful message and exit\n");
	fprintf(stdout, " -v                --version               Print version information and exit\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "Report bugs to %s\n", PACKAGE_BUGREPORT);
	fflush(stdout);

	exit(EXIT_SUCCESS);
}

static void print_version(void) {

	fprintf(stdout, "%s\n", PACKAGE_STRING);
	fprintf(stdout, "\n");
	fprintf(stdout, "Copyright (C) 2018  Thomas Cort <linuxgeek@gmail.com>\n");
	fprintf(stdout, "This is free software; see the source for copying conditions.  There is NO\n");
	fprintf(stdout, "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "Written by Thomas Cort <linuxgeek@gmail.com>\n");
	fflush(stdout);

	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {

	int optc;
	const char* const short_options = "h?v";

#if HAVE_GETOPT_LONG
	static const struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'v'},
		{NULL, 0, NULL, 0}
	};
#endif

#if HAVE_GETOPT_LONG
	while ((optc = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
#else
	while ((optc = getopt(argc, argv, short_options)) != -1) {
#endif
		switch (optc) {
			case 'v':
				print_version();
				break;
			case 'h':
			case '?':
				print_help(argv[0]);
				break;
		}
	}

	if (optind != argc) {
		print_help(argv[0]);
	}

	return EXIT_SUCCESS;
};
