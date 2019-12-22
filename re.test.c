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

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "check.h"
#include "re.h"

int main(int argc, char *argv[]) {

	(void) argc;
	(void) argv;

	check(match("Hello", "^H.l*o$") == 1, "should match");
	check(match("Hello", "Hel+o") == 1, "should match");
	check(match("Hello", "^") == 1, "should match");
	check(match("Hello", "$") == 1, "should match");
	check(match("Hello", "Hell?o") == 1, "should match");
	check(match("Hello", "Hell?o") == 1, "should match");
	check(match("Helo", "Hell?o") == 1, "should match");
	check(match("", "^$") == 1, "should match");
	check(match("", "") == 1, "should match");
	check(match("Hello", "Hello") == 1, "should match");

	check(match("Heo", "Hel+o") == 0, "should not match");
	check(match("abc", NULL) == 0, "should not match");
	check(match(NULL, "abc") == 0, "should not match");
	check(match(NULL, NULL) == 0, "should not match");
	check(match("Hola", "^H.l*o$") == 0, "should not match");

	exit(EXIT_SUCCESS);
}
