# tcedit - line-oriented text editor.
# Copyright (C) 2019, 2020  Thomas Cort <linuxgeek@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

EXTRA_DIST = README.md CHANGELOG.md CONTRIBUTING.md HACKING.md tcedit.1

bin_PROGRAMS = tcedit rtcedit

tcedit_SOURCES = \
	cmd.c cmd.h \
	config.h \
	ctx.h \
	dstring.c dstring.h \
	error.c error.h \
	io.c io.h \
	main.c \
	parse.c parse.h \
	text.c text.h \
	check.h

rtcedit_SOURCES = $(tcedit_SOURCES)

man_MANS = tcedit.1
