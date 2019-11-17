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

int sighup_fired = 0;
int sigint_fired = 0;

void dohup(int signo) {
	sighup_fired = 1;
}

void doint(int signo) {
	sigint_fired = 1;
}

void siginit(void) {
	signal(SIGHUP, dohup);
	signal(SIGINT, doint);
	signal(SIGQUIT, SIG_IGN);
}
