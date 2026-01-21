# Minishell - A Simple Bash Implementation

![Language](https://img.shields.io/badge/Language-C-blue)
![School](https://img.shields.io/badge/School-42-black)
![License](https://img.shields.io/badge/License-MIT-green)

**Minishell** is a custom implementation of a minimal Unix shell, inspired by Bash. This project was built as part of the 42 School curriculum to explore the internal workings of command-line interpreters, process management, and file descriptor manipulation in C.

---

## 📖 Table of Contents
- [Features](#-features)
- [Architecture](#-architecture)
- [Installation](#-installation)
- [Usage](#-usage)
- [Built-in Commands](#-built-in-commands)
- [Testing](#-testing)

---

## ✨ Features

Minishell supports a robust set of features found in modern shells:

### Core Functionality
- **Prompt:** Displays a custom prompt waiting for user input using `readline`.
- **History:** Maintains a history of commands (up arrow access).
- **Executable Search:** Finds and executes binaries from system `PATH` or absolute/relative paths.
- **Signal Handling:** Replicates Bash behavior for `Ctrl-C` (SIGINT), `Ctrl-\` (SIGQUIT), and `Ctrl-D` (EOF).

### Parsing & Expansion
- **Quotes:** Handles single (`'`) and double (`"`) quotes, managing meta-characters appropriately.
- **Environment Variables:** Expands variables (e.g., `$USER`, `$HOME`) and exit status (`$?`).
- **Wildcards:** Supports `*` wildcard expansion in the current working directory.

### Redirections & Pipes
- **Pipes (`|`):** Connects the output of one command to the input of the next.
- **Input (`<`):** Redirects input from a file.
- **Output (`>`):** Redirects output to a file (overwrite).
- **Append (`>>`):** Redirects output to a file (append).
- **Heredoc (`<<`):** Reads input until a delimiter is seen.

### Advanced Logic
- **Logical Operators:** Support for `&&` (AND) and `||` (OR) for conditional execution.
- **Parenthesis:** Priority grouping `(cmd1 && cmd2)` to control execution flow.

---

## 🏗 Architecture

The shell operates in a Read-Eval-Print Loop (REPL):

1.  **Lexer:** Tokenizes the raw input string, handling separators, operators, and quotes.
2.  **Parser:** Constructs a command list (`t_cli` linked list) and validates syntax (e.g., unclosed quotes or unexpected tokens).
3.  **Expander:** Processes environment variables, handles quote removal, and performs wildcard expansion.
4.  **Executor:** Traverses the command list, managing pipes via `pipe` and `fork`, applying redirections with `dup2`, and executing either built-ins or system binaries.

---

## 🛠 Installation

### Prerequisites
- **OS:** Linux or macOS.
- **Compiler:** `gcc` or `clang`.
- **Libraries:** `readline` (required for interactive input).

### Build
Clone the repository and compile:

```bash
git clone https://github.com/yourusername/minishell.git
cd minishell
make
```

*Other make targets:*
- `make re`: Rebuild from scratch.
- `make clean`: Remove object files.
- `make fclean`: Remove object files and binary.
- `make san`: Build with AddressSanitizer (`-fsanitize=address`) for debugging.

---

## 🚀 Usage

Start the shell:
```bash
./minishell
```

### Examples
**Basic Commands:**
```bash
minishell$ ls -la
minishell$ pwd
minishell$ echo "Hello World"
```

**Pipes and Redirections:**
```bash
minishell$ grep "int" < minishell.h | cat -e > types.txt
```

**Logic and Grouping:**
```bash
minishell$ ls non_existent_file || echo "File not found"
minishell$ (cd libft && make) && echo "Libft built successfully"
```

---

## 💻 Built-in Commands

Minishell includes its own implementation of the following built-ins:

| Command | Description |
| :--- | :--- |
| `echo` | Prints arguments to stdout (supports `-n`). |
| `cd` | Changes the current working directory. |
| `pwd` | Prints the current working directory. |
| `export` | Sets environment variables. |
| `unset` | Unsets environment variables. |
| `env` | Displays the current environment. |
| `exit` | Exits the shell with an optional status code. |

---

## 🧪 Testing

The project includes an automated test script to verify lexing, parsing, and execution.

Run the tests:
```bash
./tests/tests.sh
```
