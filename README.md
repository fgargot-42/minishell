*This project has been created as part of the 42 curriculum by mabarrer, fgargot.*

# minishell 🐚

> *Because writing your own shell is the best way to understand why shells exist.*

## Description

A minimal Unix shell written in C. Handles command execution, pipes, redirections, logical operators, environment variables, and a handful of built-in commands — just enough to feel like home.

## Features

- Interactive prompt with command history
- Pipes (`|`), logical operators (`&&`, `||`), and grouped commands (`(...)`)
- Redirections: `<`, `>`, `>>`, `<<` (heredoc)
- Environment variable expansion (`$VAR`, `$?`)
- Wildcard expansion (`*`)
- Built-in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Signal handling (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`)

## Instructions


```bash
make

# Run
./minishell

# piped mode
echo "ls -la | wc -l" | ./minishell
```

**Example usage:**
```bash
minishell> echo "Hello, World!"
Hello, World!
minishell> ls -la | grep .c | wc -l
42
minishell> export MY_VAR=hello && echo $MY_VAR
hello
minishell> exit
```

## Resources

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html) — main reference for shell behavior
- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/rltop.html) — interactive input and history
- Many clean shell implementation guides

