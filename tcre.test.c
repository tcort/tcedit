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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "check.h"
#include "tcre.h"

int main(int argc, char *argv[]) {

	size_t i;
	int debug;
	char *pass[][2] = {
		{ "13.37", "[123456789]+[0123456789]*\\.[0123456789][0123456789]" },
		{ "a03.37a", "[123456789]+[0123456789]*\\.[0123456789][0123456789]" },
		{ "Hello", "[Hh]ello" },
		{ "abababc", "a[ab]+c" },
		{ "Hello", "[^h]ello" },
		{ "hello", "[Hh]ello" },
		{ "Hello", "^H.l*o$" },
		{ "Hello", "Hel+o"   },
		{ "Hello", "^"       },
		{ "Hello", "$"       },
		{ "Hello", "Hell?o"  },
		{ "Hello", "Hell?o"  },
		{ "Helo",  "Hell?o"  },
		{ "",      "^$"      },
		{ "",      ""        },
		{ "Hello", "Hello"   },
		{ "\\^\\.\\*\\$", "^.*$" },
		{ "This is text", "This is text"},
		{ "a4", "[a-z0-9]+"},
		{ "z", "[a-z0-9]"},
		{ "a", "[^b-z0-9]"},
		{ "9", "[^b-z0-8]"},
		{ "-", "[-]"}
	};
#define NPASS (sizeof(pass)/sizeof(pass[0]))

	char *fail[][2] = {
		{ "a", "[b-z0-9]"},
		{ "9", "[b-z0-8]"},
		{ "yello", "[Hh]ello" },
		{ "Heo",  "Hel+o"   },
		{ "abc",  NULL      },
		{ NULL,   "abc"     },
		{ NULL,   NULL      },
		{ "Hola", "^H.l*o$" },
	};
#define NFAIL (sizeof(fail)/sizeof(fail[0]))

	if (argc == 2 && strcmp(argv[1], "-d") == 0) {
		debug = 1;
	} else {
		debug = 0;
	}

	for (i = 0; i < NPASS; i++) {
		if (debug == 1) {
			printf("subject='%s' pattern='%s'\n", pass[i][0], pass[i][1]);
		}
		check(match(pass[i][0], pass[i][1]) == 1, "should match");
	}

	for (i = 0; i < NFAIL; i++) {
		if (debug == 1) {
			printf("subject='%s' pattern='%s'\n", fail[i][0], fail[i][1]);
		}
		check(match(fail[i][0], fail[i][1]) == 0, "should not match");
	}

	exit(EXIT_SUCCESS);
}
