# tcedit

A simple text editor.

## Requirements

`tcedit` doesn't have any external dependencies. To compile it, one simply needs a standard C toolchain.

## Installation

Standard autotools build:

```
$ ./configure
$ make
$ make check
# make install
```

## Usage

```
tcedit [-p string] [-r] [-s] [-v] [filename]
```

| option      | description                                                                                         |
| ----------- | --------------------------------------------------------------------------------------------------- |
| `-p string` | Sets the command prompt to the given `string` and toggles prompting to on. Default prompt is `*`.   |
| `-r`        | Enable restricted mode (see below).                                                                 |
| `-s`        | Suppress byte counts for `e`, `E`, `r`, and `w` commands as well as the ending `!` for `!` command. |
| `-v`        | Print version string (e.g. `tcedit 1.2.3`) and exit.                                                |

## Restricted Mode

When the editor is invoked with the `-r` command line argument or when invoked as `rtcedit`,
it enters restricted mode. In restricted mode, the `!` command is disabled as well as `!` in filenames for `r` and `w`.
Additionally, only files in the current directory may be edited (i.e. filenames must not contain any `/` characters
nor the sequence `..`.

Note, some restrictions can be bypassed if the user is able to create symbolic links or hard links.

## Addressing

Lines may be addressed as follows:

* `.` - current line in the buffer
* `$` - last line in the buffer
* `,` or `%` - with no address arguments is equivalent to `1,$`
* `/regex/` - first line after the current line which matches regular expression `regex`.
* `?regex?` - first line before the current line which matches regular expression `regex`.
* _n_ - numeric line number (e.g. `42`, `15`, etc)

The following shorthand is available:

| short hand | meaning     |
| ---------- | ----------- |
| `,`        | `1,$`       |
| `,addr`    | `1,addr`    |
| `addr,`    | `addr,addr` |

## Commands

| command                            | description                                                                                                                          |
| ---------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------ |
| `!command`                         | Execute *command* with `sh -c "command"`. Prints `!` when `sh` exits.                                                                |
| `($)=`                             | Print the number of the addressed line.                                                                                              |
| `(.)a`                             | Enter append mode. Input a line with a single `.` to exit append mode.                                                               |
| `(.,.)c`                           | Change the addressed line(s) in the buffer. Equivalent to N,Md followed by Ni.                                                       |
| `(.,.)d`                           | Delete the addressed line(s) from the buffer.                                                                                        |
| `E [filename.txt]` or `E  [!command]`      | Forcefully deletes the buffer and reads the text of a file into the buffer. It sets the default file name. Unsaved changes are lost. |
| `e [filename.txt]` or `e [!command]`      | Deletes the buffer and reads the text of a file into the buffer. It sets the default file name.                                      |
| `f [filename.txt]`                 | Set the default filename. If no filename is given, print the current default filename.                                               |
| `H`                                | Toggle helpful error messages on/off. Initially the helpful error messages are turned off.                                           |
| `h`                                | Print a helpful error message for the last error.                                                                                    |
| `($)i`                             | Enter insert mode. Input a line with a single `.` to exit insert mode.                                                               |
| `(.,.)n`                           | Print lines from the buffer with line numbers.                                                                                       |
| `P`                                | Toggle command prompt on/off. Initially the prompt is off unless the `-p string` command line argument was given.                    |
| `(.,.)p`                           | Print lines from the buffer.                                                                                                         |
| `Q`                                | Force quit. Always succeeds. Unsaved changes are lost.                                                                               |
| `q`                                | Quit. Fails if there are unsaved changes. Succeeds if entered a second time with no changes in between. Unsaved changes are lost.    |
| `($)r [filename.txt]` or `($)r [!command]`   | Read the text of a file into the buffer. A filename must be given if one has not been set (either via cmd line args or `f`).         |
| `(.,.)s/pattern/replacement/`      | Perform a global find and replace.
| `(1,$)w [filename.txt]` or `(1,$)w [!command]` | Write the text to a file. A filename must be given if one has not been set (either via cmd line args or `f`).                        |
| `<newline>`                        | A blank line prints the current line and advances `.`. This is equivalent to `.+1p`.                                                 |
| `# comment`                        | A comment line. Comment lines are ignored which is useful for scripting.                                                             |

## Regular Expressions

`tcedit` supports POSIX Extended Regular Expressions.

## License

**GPLv3+**

```
/*
 * tcedit - line-oriented text editor.
 * Copyright (C) 2019, 2020  Thomas Cort <linuxgeek@gmail.com>
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
```

See `COPYING` for full details.
