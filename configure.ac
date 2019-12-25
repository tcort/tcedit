dnl    tcedit - line-oriented text editor.
dnl    Copyright (C) 2019  Thomas Cort <linuxgeek@gmail.com>
dnl
dnl    This program is free software: you can redistribute it and/or modify
dnl    it under the terms of the GNU General Public License as published by
dnl    the Free Software Foundation, either version 3 of the License, or
dnl    (at your option) any later version.
dnl
dnl    This program is distributed in the hope that it will be useful,
dnl    but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl    GNU General Public License for more details.
dnl
dnl    You should have received a copy of the GNU General Public License
dnl    along with this program.  If not, see <http://www.gnu.org/licenses/>.

AC_PREREQ(2.59)
AC_INIT([tcedit], [2019.12.25], [linuxgeek@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_LANG([C])
AC_PROG_CC
AC_GNU_SOURCE
AC_HEADER_STDC

AC_CHECK_HEADERS([malloc.h])
AC_CHECK_FUNCS([reallocarray])

AC_CONFIG_HEADERS([config.h:config.in])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
