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

#ifndef __TCE_ERROR_H
#define __TCE_ERROR_H

#define TCE_ERR_OK 0
#define TCE_ERR_UNSAVED_CHANGES 1
#define TCE_ERR_UNKNOWN_ERROR 2
#define TCE_ERR_BAD_ADDR 3
#define TCE_ERR_WRITE_FAILED 4
#define TCE_ERR_FILENAME_NOT_SET 5
#define TCE_ERR_READ_FAILED 6

extern char *tce_errlist[];
extern int tce_nerror;
extern int tce_errno;

char *tce_strerror(int code);

#endif
