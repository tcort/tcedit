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

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "sig.h"

void dohup(int signo) {
	printf("SIGHUP\n");
/* TODO if text dirty, write to 'tcedit.hup' and exit */
	exit(1);
}

void doint(int signo) {
	printf("SIGINT\n");
/* TODO  print "?\n", return to cmd mode, if in input mode keep text already input */
	exit(1);
}

void siginit(void) {
	signal(SIGHUP, dohup);
	signal(SIGINT, doint);
	signal(SIGQUIT, SIG_IGN);
}
