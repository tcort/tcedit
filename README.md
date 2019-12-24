# tcedit

A simple text editor.

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

When the editor is invoked with the `-r` command line argument or when invoked as rtcedit (i.e. `argv[0][0] == 'r'`),
it enters restricted mode. In restricted mode, the `!` command is disabled as well as `!` in filenames for `r` and `w`.

## Addressing

Lines may be addressed as follows:

* `.` - current line in the buffer
* `$` - last line in the buffer
* `,` or `%` - with no address arguments is equivalent to `1,$`
* `/regex/` - first line after the current line which matches regular expression `regex`.
* `?regex?` - first line before the current line which matches regular expression `regex`.
* _n_ - numeric line number (e.g. `42`, `15`, etc)

## Commands

| command                            | description                                                                                                                          |
| ---------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------ |
| `!command`                         | Execute *command* with `sh -c "command"`. Prints `!` when `sh` exits.                                                                |
| `($)=`                             | Print the number of the addressed line.                                                                                              |
| `(.)a`                             | Enter append mode. Input a line with a single `.` to exit append mode.                                                               |
| `(.,.)c`                           | Change the addressed line(s) in the buffer. Equivalent to N,Md followed by Ni.                                                       |
| `(.,.)d`                           | Delete the addressed line(s) from the buffer.                                                                                        |
| `E [filename.txt | !command]`      | Forcefully deletes the buffer and reads the text of a file into the buffer. It sets the default file name. Unsaved changes are lost. |
| `e [filename.txt | !command]`      | Deletes the buffer and reads the text of a file into the buffer. It sets the default file name.                                      |
| `f [filename.txt]`                 | Set the default filename. If no filename is given, print the current default filename.                                               |
| `H`                                | Toggle helpful error messages on/off. Initially the helpful error messages are turned off.                                           |
| `h`                                | Print a helpful error message for the last error.                                                                                    |
| `($)i`                             | Enter insert mode. Input a line with a single `.` to exit insert mode.                                                               |
| `(.,.)n`                           | Print lines from the buffer with line numbers.                                                                                       |
| `P`                                | Toggle command prompt on/off. Initially the prompt is off unless the `-p string` command line argument was given.                    |
| `(.,.)p`                           | Print lines from the buffer.                                                                                                         |
| `Q`                                | Force quit. Always succeeds. Unsaved changes are lost.                                                                               |
| `q`                                | Quit. Fails if there are unsaved changes. Succeeds if entered a second time with no changes in between. Unsaved changes are lost.    |
| `($)r [filename.txt | !command]`   | Read the text of a file into the buffer. A filename must be given if one has not been set (either via cmd line args or `f`).         |
| `(1,$)w [filename.txt | !command]` | Write the text to a file. A filename must be given if one has not been set (either via cmd line args or `f`).                        |
| `<newline>`                        | A blank line prints the current line and advances `.`. This is equivalent to `.+1p`.                                                 |
| `# comment`                        | A comment line. Comment lines are ignored which is useful for scripting.                                                             |

## Regular Expressions

| Pattern | Description                                            |
| ------- | ------------------------------------------------------ |
| `c`     | Matches any non-special character `c`.                 |
| `\c`    | Matches any character `c` (e.g. `\*` to match `*`).    |
| `.`     | Matches any character.                                 |
| `[abc]` | Matches any character inside the brackets.             |
| `[^xy]` | Matches any character not inside the brackets.         |
| `[A-Z]` | Matches any character in range `A` to `Z` (inclusive). |
| `^`     | Anchor matching at start of subject.                   |
| `$`     | Anchor matching at end of subject.                     |
| `?`     | Matches the preceding character 0 or 1 times.          |
| `*`     | Matches the preceding character 0 or more times.       |
| `+`     | Matches the preceding character 1 or more times.       |
