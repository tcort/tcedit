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

| command              | description                                                                                                                       |
| -------------------- | --------------------------------------------------------------------------------------------------------------------------------- |
| `!`*command*         | Execute *command* with `sh -c "command"`. Prints `!` when `sh` exits.                                                             |
| `H`                  | Toggle helpful error messages on/off. Initially the helpful error messages are turned off.                                        |
| `h`                  | Print a helpful error message for the last error.                                                                                 |
| `n`                  | Print lines from the buffer with line numbers.                                                                                    |
| `P`                  | Toggle command prompt on/off. Initially the prompt is off unless the `-p string` command line argument was given.                 |
| `p`                  | Print lines from the buffer.                                                                                                      |
| `Q`                  | Force quit. Always succeeds. Unsaved changes are lost.                                                                            |
| `q`                  | Quit. Fails if there are unsaved changes. Succeeds if entered a second time with no changes in between. Unsaved changes are lost. |
