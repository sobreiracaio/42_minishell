# Minishell

Minishell is a student project developed as part of the 42 School curriculum. The objective is to create a functional shell with a limited set of features, mimicking the behavior of a Unix shell. It is an excellent introduction to system programming, process management, and handling signals. The project emphasizes strict coding standards and low-level programming skills.

## Features

Minishell implements the following features:

- **Command Prompt**: Displays a prompt when waiting for a new command.
- **Command History**: Supports a working command history.
- **Command Execution**:
  - Searches and launches executables based on the `PATH` variable, or using relative or absolute paths.
  - Avoids using more than one global variable for signal handling, ensuring safe data access.
- **Quoting Mechanisms**:
  - Handles single quotes (`'`) to prevent interpretation of meta-characters.
  - Handles double quotes (`"`) to prevent interpretation of meta-characters except for the dollar sign (`$`).
- **Redirections**:
  - `<` redirects input.
  - `>` redirects output.
  - `<<` (heredoc) reads input until a delimiter is encountered but does not update history.
  - `>>` appends output.
- **Pipes**: Implements pipelines (`|`) to connect the output of one command to the input of another.
- **Environment Variables**: Expands variables prefixed with `$` to their corresponding values.
- **Exit Status**: Handles `$?`, which expands to the exit status of the last executed foreground pipeline.
- **Signal Handling**:
  - `Ctrl-C` displays a new prompt on a new line in interactive mode.
  - `Ctrl-D` exits the shell.
  - `Ctrl-\` does nothing in interactive mode.
- **Built-in Commands**:
  - `echo` with the `-n` option.
  - `cd` supporting relative and absolute paths.
  - `pwd` with no options.
  - `export` with no options.
  - `unset` with no options.
  - `env` with no options or arguments.
  - `exit` with no options.

## Learning Outcomes

Through Minishell, students gain a deeper understanding of:

- Process creation and management.
- Signal handling in Unix-based systems.
- The intricacies of shell scripting and command parsing.
- The importance of robust and modular code structure.

