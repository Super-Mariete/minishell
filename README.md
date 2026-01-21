# Minishell

Minishell is a robust, production-grade shell implementation in C, designed to showcase high-level systems programming skills. It moves beyond a simple academic exercise to demonstrate excellence in memory management, error handling, and software architecture.

## 🚀 Features

- **Uncompromising Stability:** Rigorously tested against edge cases to ensure reliability and zero memory leaks (verified with Valgrind).
- **Modular Architecture:** Clear separation between the lexer, AST-based parser, and execution engine.
- **Built-in Commands:** Custom implementations of `cd`, `echo`, `env`, `exit`, `export`, `pwd`, and `unset`.
- **Process Management:** Robust handling of pipes (`|`) and redirections (`<`, `>`, `<<`, `>>`).
- **Environment Management:** Dynamic expansion of environment variables.
- **Signal Handling:** Correct behavior for `Ctrl-C`, `Ctrl-D`, and `Ctrl-\`.

## 🛠️ Build & Usage

### Prerequisites
- GCC or Clang
- Make
- Readline library

### Compilation
Use the provided `Makefile` to compile the project:

```bash
make
```

To clean up object files:
```bash
make clean
```

To full clean (remove binary):
```bash
make fclean
```

### Running the Shell
Start the shell by running the executable:

```bash
./minishell
```

## 🧪 Testing

The project includes a comprehensive integration test suite.

```bash
./tests/tests.sh
```

For memory leak detection, run with Valgrind:

```bash
valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all ./minishell
```

## 🏗️ Architecture

The project is structured into three main components:

1.  **Parsing (`parsing/`):** Handles lexical analysis (tokenization) and parses tokens into an Abstract Syntax Tree (AST) equivalent (`t_cli`). Handles expansion and syntax validation.
2.  **Execution (`exec/`):** Traverses the AST and executes commands. Manages forks, pipes, and file descriptors.
3.  **Builtins (`exec/builtins/`):** Contains the logic for internal shell commands.
4.  **Libft (`libft/`):** A custom library of standard C utilities.

## 📜 Compliance

This project adheres to the strict 42 Norminette coding standard.

```