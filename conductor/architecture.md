# System Architecture

## High-Level Data Flow
The shell operates in a continuous **Read-Eval-Print Loop (REPL)**:

1.  **Input:** Reads a line from the user via `readline`.
2.  **Lexing:** Tokenizes the input string, handling quoting rules (`'` vs `"`).
3.  **Parsing:** Validates syntax and constructs the Abstract Syntax Tree (AST), represented as a linked list of `t_cli` nodes.
4.  **Expansion:** Processes environment variables (`$VAR`) and wildcards (`*`) within the AST.
5.  **Execution:** Traverses the AST, managing pipes and redirections, and executes commands.
6.  **Cleanup:** Frees resources and awaits the next input.

## Key Components

### 1. Lexer & Parser (`parsing/`)
- **Lexer:** Splits raw input into tokens (words, operators, pipes).
- **Parser:** Converts tokens into command structures (`t_cli`). Handles syntax errors (e.g., unexpected tokens).
- **Expander:**
    - **Variables:** Replaces `$ENV_VAR` with values from `t_shenv`.
    - **Wildcards:** Expands `*` to matching filenames in the current directory.

### 2. Execution Engine (`exec/`)
- **Orchestrator (`ft_execute.c`):** Determines if a command is a builtin or external binary.
- **Process Management:**
    - **Pipes:** Creates pipes between command nodes using `pipe()`.
    - **Forks:** Uses `fork()` to create child processes for external commands.
    - **Redirections:** Manages input/output streams using `dup2()` before execution.
- **Builtins:** Executed directly in the parent process (mostly) to modify shell state (e.g., `cd`, `export`, `exit`).

### 3. Signals (`parsing/signals.c`)
- **Interactive Mode:** Handles `Ctrl-C` (new prompt), `Ctrl-D` (exit), and `Ctrl-\` (ignore).
- **Blocking Mode:** Updates signal handlers during command execution to allow child processes to handle signals appropriately.

## Core Data Structures

### `t_cli` (Command Node)
Represents a single command in the pipeline.
- `args`: Array of command arguments (e.g., `{"ls", "-l", NULL}`).
- `infile` / `outfile`: File descriptors or paths for redirection.
- `pipe_in` / `pipe_out`: FDs for inter-process communication.
- `next`: Pointer to the next command in the pipeline.

### `t_shenv` (Environment)
Linked list storing environment variables.
- `key`: Variable name (e.g., "PATH").
- `value`: Variable content.
