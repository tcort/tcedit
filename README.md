# tcedit

A simple text editor.

## Usage

```
tcedit [-v] [-p string]
```

| option      | description                                                                                       |
| ----------- | ------------------------------------------------------------------------------------------------- |
| `-p string` | Sets the command prompt to the given `string` and toggles prompting to on. Default prompt is `*`. |
| `-v`        | Print version string (e.g. `tcedit 1.2.3`) and exit.                                              |

## Addressing

Lines may be addressed as follows:

* `.` - current line in the buffer
* `$` - last line in the buffer
* _n_ - numeric line number (e.g. `42`, `15`, etc)

## Commands

| command                | description                                                                                                                       |
| ---------------------- | --------------------------------------------------------------------------------------------------------------------------------- |
| `.`                    | Print the number of the current line.                                                                                             |
| `!`*command*           | Execute *command* with `sh -c "command"`. Prints `!` when `sh` exits.                                                             |
| `($)=`                 | Print the number of the addressed line.                                                                                           |
| `($)a`                 | Enter append mode. Input a line with a single `.` to exit append mode.                                                            |
| `(.,.)d`               | Delete the addressed line(s) from the buffer.                                                                                     |
| `f [filename.txt]`     | Set the default filename. If no filename is given, print the current default filename.                                            |
| `H`                    | Toggle helpful error messages on/off. Initially the helpful error messages are turned off.                                        |
| `h`                    | Print a helpful error message for the last error.                                                                                 |
| `($)i`                 | Enter insert mode. Input a line with a single `.` to exit insert mode.                                                            |
| `(.,.)n`               | Print lines from the buffer with line numbers.                                                                                    |
| `P`                    | Toggle command prompt on/off. Initially the prompt is off unless the `-p string` command line argument was given.                 |
| `(.,.)p`               | Print lines from the buffer.                                                                                                      |
| `Q`                    | Force quit. Always succeeds. Unsaved changes are lost.                                                                            |
| `q`                    | Quit. Fails if there are unsaved changes. Succeeds if entered a second time with no changes in between. Unsaved changes are lost. |
| `(1,$)r [filename.txt]`| Read the text of a file into the buffer. A filename must be given if one has not been set (either via cmd line args or `f`)
| `(1,$)w [filename.txt]`| Write the text to a file. A filename must be given if one has not been set (either via cmd line args or `f`)

## Regular Expression

| Pattern | Description                                        |
| ------- | -------------------------------------------------- |
| `c`     | Matches any non-special character `c`.             |
| `\c`    | Matches any character `c` (e.g. `\*` to match `*`. |
| `.`     | Matches any character.                             |
| `[abc]` | Matches any character inside the brackets.         |
| `[^xy]` | Matches any character not inside the brackets.     |
| `^`     | Anchor matching at start of subject.               |
| `$`     | Anchor matching at end of subject.                 |
| `?`     | Matches the preceding character 0 or 1 times.      |
| `*`     | Matches the preceding character 0 or more times.   |
| `+`     | Matches the preceding character 1 or more times.   |
