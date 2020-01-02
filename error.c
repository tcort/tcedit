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
#include "error.h"

char *tce_errlist[] = {
	"OK",
	"warning: text has changed since it was last saved",
	"error: unknown error",
	"bad address",
	"write failed",
	"filename not set",
	"read failed",
	"command not available in restricted mode",
	"process i/o not available in restricted mode",
	"missing parameter",
	"file not available in restricted mode",
	"bad substitution",
	"bad parameter"
};

int tce_nerror = sizeof(tce_errlist)/sizeof(char*);
int tce_errno = TCE_ERR_OK;

char *tce_strerror(int code) {
	if (code >= tce_nerror) {
		code = TCE_ERR_UNKNOWN_ERROR;
	}
	return tce_errlist[code];
}
