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

#ifndef __TCE_CMD_H
#define __TCE_CMD_H

#include "ctx.h"
#include "parse.h"

#define NCOMMANDS (20)

enum address_default {
	ADDR_FIRST_LINE = '1',
	ADDR_CURRENT_LINE = '.',
	ADDR_LAST_LINE = '$',
	ADDR_NEXT_LINE = 'N',
	ADDR_NONE = '0'
};

struct command {
        char letter;
        int (*action) (struct context *ctx, struct input in);
	enum address_default default_addrs[2];
};

extern struct command commands[NCOMMANDS];

#endif
